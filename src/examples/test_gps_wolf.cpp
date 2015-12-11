// Testing create and delete full wolf tree with GPS captures

//std includes
#include <iostream>
#include <memory>
#include <random>
#include <cmath>
#include <queue>

//Wolf includes
#include "wolf_manager_gps.h"
#include "ceres_wrapper/ceres_manager.h"


using namespace std;

int main(int argc, char** argv)
{
    //Welcome message
    cout << endl << " ========= WOLF TREE test ===========" << endl << endl;

    SensorGPS* gps_sensor_ptr_ = new SensorGPS(new StateBlock(Eigen::Vector3s::Zero()),
                                               new StateBlock(Eigen::Vector4s::Zero()),
                                               new StateBlock(Eigen::Vector1s::Zero()));


    WolfManagerGPS* wolf_manager_ = new WolfManagerGPS(PO_3D,                             //frame structure
                                                         nullptr,                   //gps raw sensor
                                                         Eigen::Vector7s::Zero(),           //prior
                                                         Eigen::Matrix7s::Identity()*0.01,  //prior cov
                                                         5,                                 //window size
                                                         1);                                //time for new keyframe

    // Ceres wrapper
    ceres::Solver::Options ceres_options;
    ceres_options.minimizer_type = ceres::TRUST_REGION; //ceres::TRUST_REGION;LINE_SEARCH
    ceres_options.max_line_search_step_contraction = 1e-3;
    ceres_options.max_num_iterations = 1e4;
    ceres::Problem::Options problem_options;
    problem_options.cost_function_ownership = ceres::TAKE_OWNERSHIP;
    problem_options.loss_function_ownership = ceres::TAKE_OWNERSHIP;
    problem_options.local_parameterization_ownership = ceres::TAKE_OWNERSHIP;
    CeresManager* ceres_manager = new CeresManager(wolf_manager_->getProblemPtr(), problem_options);





    wolf_manager_->addSensor(gps_sensor_ptr_);


    for(int i=0; i<20; ++i)
    {
        cout << "%%%%%%%%%%%%%%%%%%%%%% CAPTURE #" << i << endl;
        TimeStamp time_stamp(i);

        Eigen::VectorXs raw_data(3);
        raw_data << 42,43,44;



        // Create synthetic gps capture
        CaptureGPS* cpt_ptr_ = new CaptureGPS(time_stamp, gps_sensor_ptr_, raw_data);

        // Add capture
        wolf_manager_->addCapture(cpt_ptr_);
        //cout << "capture added" << endl;

        // update wolf tree
        wolf_manager_->update();
        //cout << "wolf manager updated" << endl;

        ceres_manager->update();
        //cout << "ceres manager updated" << endl;

        ceres::Solver::Summary summary;

        summary = ceres_manager->solve(ceres_options);
        cout << summary.FullReport() << endl;
    }


    delete wolf_manager_; //not necessary to delete anything more, wolf will do it!

    //End message
    cout << " =========================== END ===============================" << std::endl << std::endl;

    //exit
    return 0;
}

