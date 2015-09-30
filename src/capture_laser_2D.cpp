#include "capture_laser_2D.h"

// unsigned int CaptureLaser2D::segment_window_size = 8;//window size to extract segments
// double CaptureLaser2D::theta_min = 0.4; //minimum theta between consecutive segments to detect corner. PI/8=0.39
// double CaptureLaser2D::theta_max_parallel = 0.1; //maximum theta between consecutive segments to fuse them in a single line.
// double CaptureLaser2D::k_sigmas = 3.;//How many std_dev are tolerated to count that a point is supporting a line
// unsigned int CaptureLaser2D::max_beam_distance = 5;//max number of beams of distance between lines to consider corner or concatenation
// double CaptureLaser2D::max_distance = 0.5;//max distance between line ends to consider corner or concatenation

//CaptureLaser2D::CaptureLaser2D(const TimeStamp & _ts, SensorBase* _sensor_ptr, const std::vector<WolfScalar>& _ranges):
//	CaptureBase(_ts, _sensor_ptr, _ranges),
//	ranges_(data_.data(), _ranges.size()),
//	intensities_(data_.data(), 0)
//{
//    laser_ptr_ = (SensorLaser2D*)sensor_ptr_;
//}
CaptureLaser2D::CaptureLaser2D(const TimeStamp & _ts, 
                               SensorBase* _sensor_ptr, 
                               const std::vector<float>& _ranges) 
                               :
                               CaptureBase(_ts, _sensor_ptr), 
                               ranges_(_ranges)
{
    laser_ptr_ = (SensorLaser2D*) sensor_ptr_;
}


//CaptureLaser2D::CaptureLaser2D(const TimeStamp & _ts, SensorBase* _sensor_ptr, const Eigen::VectorXs& _ranges, const Eigen::VectorXs& _intensities):
//    CaptureBase(_ts, _sensor_ptr, _ranges),
//    ranges_(data_.data(), _ranges.size()),
//    intensities_(data_.data(), _intensities.size())
//{
//      laser_ptr_ = (SensorLaser2D*)sensor_ptr_;
//}
CaptureLaser2D::CaptureLaser2D(const TimeStamp & _ts, 
                               SensorBase* _sensor_ptr, 
                               const std::vector<float>& _ranges, 
                               const std::vector<float>& _intensities) 
                               :
                               CaptureBase(_ts, _sensor_ptr),
                               ranges_(_ranges), 
                               intensities_(_intensities)
{
    laser_ptr_ = (SensorLaser2D*) sensor_ptr_;
}


CaptureLaser2D::~CaptureLaser2D()
{
    //
}

void CaptureLaser2D::processCapture()
{
    //variables
    std::list <laserscanutils::Corner> corners;

    //extract corners from range data
    this->extractCorners(corners);
    //std::cout << corners.size() << " corners extracted" << std::endl;

    //generate a feature for each corner
    this->createFeatures(corners);
    //std::cout << getFeatureListPtr()->size() << " Features created" << std::endl;

    //Establish constraints for each feature
    //establishConstraints();
    this->establishConstraintsMHTree();
    //std::cout << "Constraints created" << std::endl;
}

unsigned int CaptureLaser2D::extractCorners(std::list<laserscanutils::Corner> & _corner_list) const
{
//     std::list < laserscanutils::Line > line_list;
//     
//     laserscanutils::extractLines(laser_ptr_->getScanParams(), laser_ptr_->getCornerAlgParams(), ranges_, line_list);
//     return laserscanutils::extractCorners(laser_ptr_->getScanParams(), laser_ptr_->getCornerAlgParams(), line_list, _corner_list);
    return laserscanutils::extractCorners(laser_ptr_->getScanParams(), laser_ptr_->getCornerAlgParams(), ranges_, _corner_list);
}

unsigned int CaptureLaser2D::extractLines(std::list<laserscanutils::Line> & _line_list) const
{
    return laserscanutils::extractLines(laser_ptr_->getScanParams(), laser_ptr_->getCornerAlgParams().line_params_, ranges_, _line_list);
}

void CaptureLaser2D::createFeatures(std::list<laserscanutils::Corner> & _corner_list)
{
    // TODO: Sensor model
    Eigen::Matrix4s cov_mat;
    Eigen::Vector4s meas;

    //init constant cov
    cov_mat << 0.01, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 0.01;

    //for each corner in the list create a feature
    for (auto corner_it = _corner_list.begin(); corner_it != _corner_list.end(); corner_it++)
    {
        meas.head(2) = (*corner_it).pt_.head(2);
        meas(2) = (*corner_it).orientation_;
        meas(3) = (*corner_it).aperture_;
        //TODO: add the rest of descriptors and struct atributes
        this->addFeature((FeatureBase*) (new FeatureCorner2D(meas, cov_mat)));
    }
}

void CaptureLaser2D::establishConstraints()
{   
    // Global transformation TODO: Change by a function
    Eigen::Vector2s t_robot = getFramePtr()->getPPtr()->getVector();
    Eigen::Matrix2s R_robot = ((StateOrientation*) (getFramePtr()->getOPtr()))->getRotationMatrix().topLeftCorner<2, 2>();
    WolfScalar& robot_orientation = *(getFramePtr()->getOPtr()->getPtr());

    // Sensor transformation
    Eigen::Vector2s t_sensor = getSensorPtr()->getPPtr()->getVector().head(2);
    Eigen::Matrix2s R_sensor = getSensorPtr()->getOPtr()->getRotationMatrix().topLeftCorner<2, 2>();

    //Brute force closest (xy and theta) landmark search //TODO: B&B
//    std::cout << "Brute force closest (xy and theta) landmark search: N features:" << getFeatureListPtr()->size() << std::endl;
//  std::cout << "N landmark:" << getTop()->getMapPtr()->getLandmarkListPtr()->size() << std::endl;
//  std::cout << "Vehicle transformation: " << std::endl;
//  std::cout << "t: " << t.transpose() << std::endl;
//  std::cout << "rot:" << R << std::endl;
    for (auto feature_it = getFeatureListPtr()->begin(); feature_it != getFeatureListPtr()->end(); feature_it++)
    {
        WolfScalar max_distance_matching_sq = 0.5;
        WolfScalar max_theta_matching = M_PI / 16;
        WolfScalar min_distance_sq = max_distance_matching_sq;

        //Find the closest landmark to the feature
        LandmarkBase* correspondent_landmark = nullptr;
        const Eigen::Vector2s& feature_position = (*feature_it)->getMeasurement().head(2);
        const WolfScalar& feature_orientation = (*feature_it)->getMeasurement()(2);
        const WolfScalar& feature_aperture = (*feature_it)->getMeasurement()(3);

        Eigen::Vector2s feature_global_position = R_robot * (R_sensor * feature_position + t_sensor) + t_robot;
        WolfScalar feature_global_orientation = feature_orientation + robot_orientation + atan2(R_sensor(1, 0), R_sensor(0, 0));
        feature_global_orientation = (feature_global_orientation > 0 ? // fit in (-pi, pi]
                                    fmod(feature_global_orientation + M_PI, 2 * M_PI) - M_PI : fmod(feature_global_orientation - M_PI, 2 * M_PI) + M_PI);

//        std::cout << "-------- Feature: " << (*feature_it)->nodeId() << std::endl << feature_global_position.transpose() << "\t" << feature_global_orientation
//                << "\t" << feature_aperture << std::endl;
        for (auto landmark_it = getTop()->getMapPtr()->getLandmarkListPtr()->begin(); landmark_it != getTop()->getMapPtr()->getLandmarkListPtr()->end(); landmark_it++)
        {
            Eigen::Map<Eigen::Vector2s> landmark_position((*landmark_it)->getPPtr()->getPtr());
            WolfScalar& landmark_orientation = *((*landmark_it)->getOPtr()->getPtr());
            const WolfScalar& landmark_aperture = (*landmark_it)->getDescriptor()(0);

            // First check: APERTURE
            // std::cout << " aperture diff: " << fabs(feature_aperture - landmark_aperture);
            if (fabs(feature_aperture - landmark_aperture) < max_theta_matching)
            {
                //std::cout << " OK!" << std::endl;

                // MAHALANOBIS DISTANCE
                WolfScalar mahalanobis_distance = computeMahalanobisDistance((*feature_it),(*landmark_it));

                // Second check: SQUARED DISTANCE
                WolfScalar distance_sq = (landmark_position - feature_global_position).squaredNorm();
                //std::cout << " distance squared: " << distance_sq;
                if (distance_sq < min_distance_sq)
                {
//                      std::cout << "Close landmark candidate: " << (*landmark_it)->nodeId() << std::endl <<
//                                   landmark_position.transpose() <<
//                                   "\t" << landmark_orientation <<
//                                   "\t" << landmark_aperture << std::endl;
                    //std::cout << " OK!" << std::endl;
                    // Third check: ORIENTATION
                    WolfScalar theta_distance = fabs(fmod(fabs(landmark_orientation - feature_global_orientation) + M_PI, 2 * M_PI) - M_PI); // fit in (-pi, pi]

//                  std::cout << " orientation diff: " << theta_distance;
                    if (theta_distance < max_theta_matching)
                    {
            //            std::cout << " OK!" << std::endl;
            //            std::cout << "Closer landmark found (satisfying orientation and aperture)" << std::endl;

                        correspondent_landmark = (*landmark_it);
                        min_distance_sq = distance_sq;
                    }
//          else
//            std::cout << " KO" << std::endl;
//        else
//          std::cout << " KO" << std::endl;
                }
            }
//      else
//        std::cout << " KO" << std::endl;
        }
        if (correspondent_landmark == nullptr)
        {
//          std::cout << "+++++ No landmark found. Creating a new one..." << std::endl;
            StateBase* new_landmark_state_position = new StatePoint2D(getTop()->getNewStatePtr());
            getTop()->addState(new_landmark_state_position, feature_global_position);
            StateOrientation* new_landmark_state_orientation = new StateTheta(getTop()->getNewStatePtr());
            getTop()->addState(new_landmark_state_orientation, Eigen::Map < Eigen::Vector1s > (&feature_global_orientation, 1));

            correspondent_landmark = (LandmarkBase*)new LandmarkCorner2D(new_landmark_state_position, new_landmark_state_orientation, feature_aperture);
            getTop()->getMapPtr()->addLandmark(correspondent_landmark);

//          std::cout << "Landmark created: " <<
//                       getTop()->getMapPtr()->getLandmarkListPtr()->back()->nodeId() << std::endl <<
//                       feature_global_position.transpose() <<
//                       "\t" << feature_global_orientation <<
//                       "\t" << feature_aperture << std::endl;

        }
        else
        {
            correspondent_landmark->hit();
        }

        // std::cout << "Creating new constraint: Landmark " << getTop()->getMapPtr()->getLandmarkListPtr()->back()->nodeId() << " & feature "
        //        << (*feature_it)->nodeId() << std::endl;

        // Add constraint to the correspondent landmark
        (*feature_it)->addConstraint(new ConstraintCorner2DTheta(*feature_it, (LandmarkCorner2D*)correspondent_landmark, getFramePtr()->getPPtr(),                 //_robotPPtr,
                getFramePtr()->getOPtr(),                   //_robotOPtr,
                correspondent_landmark->getPPtr(), //_landmarkPPtr,
                correspondent_landmark->getOPtr())); //_landmarkOPtr,
    }
}


void CaptureLaser2D::establishConstraintsMHTree()
{   
    //local declarations
    WolfScalar prob, dm2, apert_diff;
    unsigned int ii, jj, kk;
    std::vector<std::pair<unsigned int, unsigned int> > ft_lk_pairs;
    std::vector<bool> associated_mask;
    Eigen::Vector2s feature_global_position;
    WolfScalar feature_global_orientation; 
    LandmarkBase* correspondent_landmark = nullptr;
    
    // Global transformation TODO: Change by a function
    Eigen::Vector2s t_robot = getFramePtr()->getPPtr()->getVector();
    Eigen::Matrix2s R_robot = ((StateOrientation*) (getFramePtr()->getOPtr()))->getRotationMatrix().topLeftCorner<2,2>();
    WolfScalar& robot_orientation = *(getFramePtr()->getOPtr()->getPtr());//TODO: it only works for orientation theta

    // Sensor transformation
    Eigen::Vector2s t_sensor = getSensorPtr()->getPPtr()->getVector().head(2);
    Eigen::Matrix2s R_sensor = getSensorPtr()->getOPtr()->getRotationMatrix().topLeftCorner<2,2>();

    //tree object allocation and sizing
    AssociationTree tree;
    tree.resize( getFeatureListPtr()->size() , getTop()->getMapPtr()->getLandmarkListPtr()->size() );

    //set independent probabilities between feature-landmark pairs
    ii=0;
    for (auto i_it = getFeatureListPtr()->begin(); i_it != getFeatureListPtr()->end(); i_it++, ii++) //ii runs over extracted feature
    {
        jj = 0;
        for (auto j_it = getTop()->getMapPtr()->getLandmarkListPtr()->begin(); j_it != getTop()->getMapPtr()->getLandmarkListPtr()->end(); j_it++, jj++)
        {
            if ((*j_it)->getType() == LANDMARK_CORNER)
            {
                //If aperture difference is small enough, proceed with Mahalanobis distance. Otherwise Set prob to 0 to force unassociation
                apert_diff = fabs( (*i_it)->getMeasurement(3) - (*j_it)->getDescriptor(0) );
                if (apert_diff < MAX_ACCEPTED_APERTURE_DIFF)
                {
                    dm2 = computeMahalanobisDistance(*i_it, *j_it);//Mahalanobis squared
                    if (dm2 < 5*5)
                        prob = erfc( sqrt(dm2)/1.4142136 );// sqrt(2) = 1.4142136
                    else
                        prob = 0;
                    tree.setScore(ii,jj,prob);
                }
                else
                    tree.setScore(ii,jj,0.);//prob to 0
            }
            else if ((*j_it)->getType() == LANDMARK_CONTAINER)
            {
                //TODO
            }
        }
    }
    
    // Grows tree and make association pairs
    tree.solve(ft_lk_pairs,associated_mask);

    //print tree & score table 
//     std::cout << "-------------" << std::endl;
//     tree.printTree();
//     tree.printScoreTable();

    //loop over all features
    ii = 0; //runs over features
    kk = 0; //runs over established pairs
    for (auto i_it = getFeatureListPtr()->begin(); i_it != getFeatureListPtr()->end(); i_it++, ii++) //ii runs over extracted feature
    {
        if ( associated_mask[ii] == false ) //unassociated feature case
        {
            //get feature on sensor frame
            const Eigen::Vector2s& feature_position = (*i_it)->getMeasurement().head(2);
            const WolfScalar& feature_orientation = (*i_it)->getMeasurement()(2);
            const WolfScalar& feature_aperture = (*i_it)->getMeasurement()(3);
            
            //translate feature position and orientation to world (global)
            feature_global_position = R_robot * (R_sensor * feature_position + t_sensor) + t_robot;
            feature_global_orientation = feature_orientation + robot_orientation + atan2(R_sensor(1, 0), R_sensor(0, 0));
            feature_global_orientation = (feature_global_orientation > 0 ? // fit in (-pi, pi]
                                            fmod(feature_global_orientation + M_PI, 2 * M_PI) - M_PI : 
                                            fmod(feature_global_orientation - M_PI, 2 * M_PI) + M_PI);
            
            //create new landmark at global coordinates
            StateBase* new_landmark_state_position = new StatePoint2D(getTop()->getNewStatePtr());
            getTop()->addState(new_landmark_state_position, feature_global_position);
            StateOrientation* new_landmark_state_orientation = new StateTheta(getTop()->getNewStatePtr());
            getTop()->addState(new_landmark_state_orientation, Eigen::Map < Eigen::Vector1s > (&feature_global_orientation, 1));
                        
            //add it to the slam map as a new landmark
            correspondent_landmark = new LandmarkCorner2D(new_landmark_state_position, new_landmark_state_orientation, feature_aperture);
            getTop()->getMapPtr()->addLandmark(correspondent_landmark);

            // Try to build a container
            tryContainer((LandmarkCorner2D*)correspondent_landmark);
        }
        else //feature-landmark association case
        {
            // get the landmark iterator corresponding to the pair
            jj=0; 
            for (auto j_it = getTop()->getMapPtr()->getLandmarkListPtr()->begin(); j_it != getTop()->getMapPtr()->getLandmarkListPtr()->end(); j_it++, jj++)
            {
                if ( jj == ft_lk_pairs[kk].second ) 
                {
                    correspondent_landmark = (*j_it);
                    correspondent_landmark->hit();
                    break; 
                }
            }
            
            //increment pair index
            kk++;
        }
        
        // Add constraint to the correspondent landmark
        (*i_it)->addConstraint(new ConstraintCorner2DTheta(
                            *i_it,                      //feature pointer
                            (LandmarkCorner2D*)correspondent_landmark,     //landmark pointer
                            getFramePtr()->getPPtr(),       //_robotPPtr,
                            getFramePtr()->getOPtr(),       //_robotOPtr,
                            correspondent_landmark->getPPtr(),   //_landmarkPPtr,
                            correspondent_landmark->getOPtr())); //_landmarkOPtr,
    }  
}

Eigen::VectorXs CaptureLaser2D::computePrior(const TimeStamp& _now) const
{
    return Eigen::Vector3s(1, 2, 3);
}

WolfScalar CaptureLaser2D::computeMahalanobisDistance(const FeatureBase* _feature_ptr, const LandmarkBase* _landmark_ptr)
{
    FrameBase* frame_ptr = _feature_ptr->getFramePtr();
    unsigned int frame_p_size = frame_ptr->getPPtr()->getStateSize();
    unsigned int frame_o_size = frame_ptr->getOPtr()->getStateSize();
    unsigned int landmark_p_size = _landmark_ptr->getPPtr()->getStateSize();
    unsigned int landmark_o_size = _landmark_ptr->getOPtr()->getStateSize();

    Eigen::Vector2s p_robot = getFramePtr()->getPPtr()->getVector();
    Eigen::Matrix2s R_robot = ((StateOrientation*) (getFramePtr()->getOPtr()))->getRotationMatrix().topLeftCorner<2,2>();
    Eigen::Vector2s p_sensor = getSensorPtr()->getPPtr()->getVector().head(2);
    Eigen::Matrix2s R_sensor = getSensorPtr()->getOPtr()->getRotationMatrix().topLeftCorner<2, 2>();
    Eigen::Vector2s p_landmark = _landmark_ptr->getPPtr()->getVector();

    // Fill sigma matrix (upper diagonal only)
    Eigen::MatrixXs Sigma(frame_p_size + frame_o_size + landmark_p_size + landmark_o_size, frame_p_size + frame_o_size + landmark_p_size + landmark_o_size);

    Eigen::Map<Eigen::MatrixXs> frame_p_frame_p_cov(&Sigma(0,0),frame_p_size,frame_p_size);
    Eigen::Map<Eigen::MatrixXs> frame_p_frame_o_cov(&Sigma(0,frame_p_size),frame_p_size,frame_o_size);
    Eigen::Map<Eigen::MatrixXs> frame_p_landmark_p_cov(&Sigma(0,frame_p_size+frame_o_size),frame_p_size,landmark_p_size);
    Eigen::Map<Eigen::MatrixXs> frame_p_landmark_o_cov(&Sigma(0,frame_p_size+frame_o_size+landmark_p_size),frame_p_size,landmark_o_size);

    getTop()->getCovarianceBlock(frame_ptr->getPPtr(), frame_ptr->getPPtr(), frame_p_frame_p_cov);
    getTop()->getCovarianceBlock(frame_ptr->getPPtr(), frame_ptr->getOPtr(), frame_p_frame_o_cov);
    getTop()->getCovarianceBlock(frame_ptr->getPPtr(), _landmark_ptr->getPPtr(), frame_p_landmark_p_cov);
    getTop()->getCovarianceBlock(frame_ptr->getPPtr(), _landmark_ptr->getOPtr(), frame_p_landmark_o_cov);

    Eigen::Map<Eigen::MatrixXs> frame_o_frame_o_cov(&Sigma(frame_p_size,frame_p_size),frame_o_size,frame_o_size);
    Eigen::Map<Eigen::MatrixXs> frame_o_landmark_p_cov(&Sigma(frame_p_size,frame_p_size+frame_o_size),frame_o_size,landmark_p_size);
    Eigen::Map<Eigen::MatrixXs> frame_o_landmark_o_cov(&Sigma(frame_p_size,frame_p_size+frame_o_size+landmark_p_size),frame_o_size,landmark_o_size);

    getTop()->getCovarianceBlock(frame_ptr->getOPtr(), frame_ptr->getOPtr(), frame_o_frame_o_cov);
    getTop()->getCovarianceBlock(frame_ptr->getOPtr(), _landmark_ptr->getPPtr(), frame_o_landmark_p_cov);
    getTop()->getCovarianceBlock(frame_ptr->getOPtr(), _landmark_ptr->getOPtr(), frame_o_landmark_o_cov);

    Eigen::Map<Eigen::MatrixXs> landmark_p_landmark_p_cov(&Sigma(frame_p_size+frame_o_size,frame_p_size+frame_o_size),landmark_p_size,landmark_p_size);
    Eigen::Map<Eigen::MatrixXs> landmark_p_landmark_o_cov(&Sigma(frame_p_size+frame_o_size,frame_p_size+frame_o_size+landmark_p_size),landmark_p_size,landmark_o_size);

    getTop()->getCovarianceBlock(_landmark_ptr->getPPtr(), _landmark_ptr->getPPtr(), landmark_p_landmark_p_cov);
    getTop()->getCovarianceBlock(_landmark_ptr->getPPtr(), _landmark_ptr->getOPtr(), landmark_p_landmark_o_cov);

    Eigen::Map<Eigen::MatrixXs> landmark_o_landmark_o_cov(&Sigma(frame_p_size+frame_o_size+landmark_p_size,frame_p_size+frame_o_size+landmark_p_size),landmark_o_size,landmark_o_size);
    getTop()->getCovarianceBlock(_landmark_ptr->getOPtr(), _landmark_ptr->getOPtr(), landmark_o_landmark_o_cov);

    // Jacobian
    if (_landmark_ptr->getOPtr()->getStateType() == ST_THETA && frame_ptr->getOPtr()->getStateType() == ST_THETA)
    {
        Eigen::MatrixXs J = Eigen::MatrixXs::Zero(6,3);
        WolfScalar theta_s_r = *getSensorPtr()->getOPtr()->getPtr() + *frame_ptr->getOPtr()->getPtr(); // Sum of theta_sensor and theta_robot

        J(0,0) = -cos(theta_s_r);
        J(0,1) =  sin(theta_s_r);
        J(1,0) = -sin(theta_s_r);
        J(1,1) = -cos(theta_s_r);
        J(2,0) = -sin(theta_s_r) * (p_landmark(0) - p_robot(0)) - cos(theta_s_r) * (p_landmark(1) - p_robot(1));
        J(2,1) =  cos(theta_s_r) * (p_landmark(0) - p_robot(0)) - sin(theta_s_r) * (p_landmark(1) - p_robot(1));
        J(2,2) = -1;
        J(3,0) =  cos(theta_s_r);
        J(3,1) = -sin(theta_s_r);
        J(4,0) =  sin(theta_s_r);
        J(4,1) =  cos(theta_s_r);
        J(5,2) =  1;

        // Feature-Landmark (euclidean) distance
        const Eigen::Vector2s& feature_position = _feature_ptr->getMeasurement().head(2);
        const WolfScalar& feature_orientation = _feature_ptr->getMeasurement()(2);

        Eigen::Vector2s landmark_local_position = R_sensor.transpose() * (R_robot.transpose() * (p_landmark - p_robot) - p_sensor);
        WolfScalar landmark_local_orientation = *_landmark_ptr->getOPtr()->getPtr() - *frame_ptr->getOPtr()->getPtr() - *getSensorPtr()->getOPtr()->getPtr();

        Eigen::Vector3s d_euclidean;
        d_euclidean.head(2) = feature_position - landmark_local_position;
        d_euclidean(2) = feature_orientation - landmark_local_orientation;
        // fit in (-pi, pi]
        d_euclidean(2) = (d_euclidean(2) > 0 ? fmod(d_euclidean(2) + M_PI, 2 * M_PI) - M_PI : fmod(d_euclidean(2) - M_PI, 2 * M_PI) + M_PI);

        // Feature-Landmark Mahalanobis distance
        // covariance: Eigen::Matrix3s S_m = J * Sigma.selfadjointView<Eigen::Upper>() * J.transpose() + _feature_ptr->getMeasurementCovariance().topLeftCorner<3,3>();
        WolfScalar mahalanobis_distance = d_euclidean.transpose() * (J.transpose() * Sigma.selfadjointView<Eigen::Upper>() * J + _feature_ptr->getMeasurementCovariance().topLeftCorner<3,3>()).inverse() * d_euclidean;
        //std::cout << "mahalanobis_distance = " << mahalanobis_distance << std::endl;
        return mahalanobis_distance;
    }
    else
    {
        std::cout << "ERROR: Jacobian using complex angles not computed..." << std::endl; //TODO
        assert(false);
        return 0;
    }
}

void CaptureLaser2D::tryContainer(LandmarkCorner2D* _corner_ptr)
{
    //std::cout << "Trying container... aperture = " << _corner_ptr->getAperture() << std::endl;
    LandmarkContainer* container_ptr = nullptr;

    // It has to be 90º corner
    if (std::fabs(_corner_ptr->getAperture() - 3 * M_PI / 2) < MAX_ACCEPTED_APERTURE_DIFF)
    {
        //std::cout << "   90º OK..." << std::endl;
        // Check all existing corners searching a container
        for (auto landmark_it = getTop()->getMapPtr()->getLandmarkListPtr()->rbegin(); landmark_it != getTop()->getMapPtr()->getLandmarkListPtr()->rend(); landmark_it++)
        {
            if ((*landmark_it)->nodeId() == _corner_ptr->nodeId())
                continue;

            //std::cout << "   landmark " << (*landmark_it)->nodeId();
            // should be a 90º corner
            if ((*landmark_it)->getType() == LANDMARK_CORNER && std::fabs(((LandmarkCorner2D*)(*landmark_it))->getAperture() - 3 * M_PI / 2) < MAX_ACCEPTED_APERTURE_DIFF)
            {
                //std::cout << "   90º OK..." << std::endl;
                WolfScalar MAX_ACCEPTED_DISTANCE_DIFF = 0.5;
                WolfScalar angle_diff = (*landmark_it)->getOPtr()->getYaw() - _corner_ptr->getOPtr()->getYaw();
                Eigen::Vector2s v_diff = (*landmark_it)->getPPtr()->getVector() - _corner_ptr->getPPtr()->getVector();
                //std::cout << "  Angle difference = " << angle_diff << std::endl;
                //std::cout << "  position difference = " << v_diff.transpose() << std::endl;

                // Large side
                if (abs(v_diff.norm() - 12.2) < MAX_ACCEPTED_DISTANCE_DIFF)
                {
                    // counterclockwise order
                    if (abs(pi2pi(angle_diff - M_PI/2)) < MAX_ACCEPTED_APERTURE_DIFF &&
                        abs(pi2pi(atan2(v_diff(1), v_diff(0)) - _corner_ptr->getOPtr()->getYaw())) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   next counterclockwise large container side detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, _corner_ptr, (LandmarkCorner2D*)(*landmark_it), nullptr, nullptr);

                        break;
                    }

                    // clockwise order
                    else if (abs(pi2pi(angle_diff + M_PI/2)) < MAX_ACCEPTED_APERTURE_DIFF &&
                             abs(pi2pi(atan2(-v_diff(1), -v_diff(0)) - (*landmark_it)->getOPtr()->getYaw())) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   prev counterclockwise large container side detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, (LandmarkCorner2D*)(*landmark_it), _corner_ptr, nullptr, nullptr);

                        break;
                    }
                }
                // Short side
                else if (abs(v_diff.norm() - 2.44) < MAX_ACCEPTED_DISTANCE_DIFF)
                {
                    // counterclockwise order
                    if (abs(pi2pi(angle_diff - M_PI/2)) < MAX_ACCEPTED_APERTURE_DIFF &&
                        abs(pi2pi(atan2(v_diff(1), v_diff(0)) - _corner_ptr->getOPtr()->getYaw())) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   next counterclockwise short container side detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, nullptr, _corner_ptr, (LandmarkCorner2D*)(*landmark_it), nullptr);

                        break;
                    }

                    // clockwise order
                    else if (abs(pi2pi(angle_diff + M_PI/2)) < MAX_ACCEPTED_APERTURE_DIFF &&
                             abs(pi2pi(atan2(-v_diff(1), -v_diff(0)) - (*landmark_it)->getOPtr()->getYaw())) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   prev counterclockwise short container side detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, nullptr, (LandmarkCorner2D*)(*landmark_it), _corner_ptr, nullptr);

                        break;
                    }
                }
                // opposite corners
                else if (abs(pi2pi(angle_diff - M_PI)) < MAX_ACCEPTED_APERTURE_DIFF &&
                         abs(v_diff.norm() - 12.4416) < MAX_ACCEPTED_DISTANCE_DIFF)
                {
                    // large side aligned corners
                    if (abs(pi2pi(atan2(v_diff(1), v_diff(0)) - _corner_ptr->getOPtr()->getYaw() - 0.1974)) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   opposite corners (aligned to large sides) detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, (LandmarkCorner2D*)(*landmark_it), nullptr, _corner_ptr, nullptr);

                        break;
                    }
                    else if (abs(pi2pi(atan2(v_diff(1), v_diff(0)) - _corner_ptr->getOPtr()->getYaw() - 1.3734)) < MAX_ACCEPTED_APERTURE_DIFF)
                    {
                        std::cout << "   opposite corners (aligned to short sides) detected!" << std::endl;
                        //create new landmark at global coordinates
                        StateBase* new_container_position = new StatePoint2D(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_position, Eigen::Vector2s::Zero());
                        StateOrientation* new_container_orientation = new StateTheta(getTop()->getNewStatePtr());
                        getTop()->addState(new_container_orientation, Eigen::Vector1s::Zero());

                        //add it to the slam map as a new landmark
                        container_ptr = new LandmarkContainer(new_container_position, new_container_orientation, nullptr, (LandmarkCorner2D*)(*landmark_it), nullptr, _corner_ptr);

                        break;
                    }
                }
            }
        }
    }

    if (container_ptr != nullptr)
        getTop()->getMapPtr()->addLandmark((LandmarkBase*)(container_ptr));
}

WolfScalar CaptureLaser2D::pi2pi(const WolfScalar& angle) const
{
    return (angle > 0 ? fmod(angle + M_PI, 2 * M_PI) - M_PI : fmod(angle - M_PI, 2 * M_PI) + M_PI);
}
