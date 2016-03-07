
// Testing things for the 3D image odometry

//Wolf includes
#include "sensor_camera.h"
#include "capture_image.h"
#include "feature_point.h"
#include "processor_ORB.h"
#include "state_block.h"

// general includes
#include "unistd.h"

//std includes
#include <iostream>


int main(int argc, char** argv)
{
    if (argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 1)
    {
        std::cout << "Please call me with: [./test_image_ORB IS_VIDEO], where:" << std::endl;
        std::cout << "    IS_VIDEO is 0 for image and 1 for video" << std::endl;
        std::cout << "EXIT due to bad user input" << std::endl << std::endl;
        return -1;
    }

    // auxiliar variables
    bool image_or_video = (atoi(argv[1]) == 1);

    //Welcome message
    std::cout << std::endl << " ========= WOLF IMAGE test ===========" << std::endl << std::endl;

    int img_width = 640;
    int img_height = 480;

    //SensorCamera test
    std::cout << std::endl << " ========= SensorCamera test ===========" << std::endl << std::endl;

    Eigen::Vector4s k = {320,240,320,320};

    StateBlock* intr = new StateBlock(k,false);

    SensorCamera* sen_cam_ = new SensorCamera(new StateBlock(Eigen::Vector3s::Zero()), new StateBlock(Eigen::Vector3s::Zero()),
                                              intr,img_width,img_height);
    std::cout << "image sensor created!" << std::endl;
    std::cout << "intr_test" << std::endl << intr->getVector() << std::endl;
    std::cout << "sen_cam intr_ptr" << std::endl << sen_cam_->getIntrinsicPtr()->getVector() << std::endl;


//    //CaptureImage test

//    set path to image location
    const char * path="/home/datchuth/Public/WP_20160211_10_19_31_Pro.jpg";
    cv::Mat image_cv =cv::imread(path);
    TimeStamp t = 1;

//    //FeaturePoint test
//    std::cout << std::endl << " ========= FeaturePoint test ===========" << std::endl << std::endl;

//    Eigen::Vector2s m_pos = {5,6};
//    Eigen::Matrix2s m_cov(2,2);
//    m_cov(0,0)=1;
//    m_cov(0,1)=2;
//    m_cov(1,0)=3;
//    m_cov(1,1)=4;

//    FeaturePoint* feat_point = new FeaturePoint(m_pos,m_cov);
//    std::cout << "measurement position: " << m_pos(0) << " " << m_pos(1) << std::endl;
//    std::cout << "measurement covariance:\n" << m_cov(0,0) << " " << m_cov(0,1) << "\n" << m_cov(1,0) << " " << m_cov(1,1) << std::endl;
//    std::cout << "feature position:\n" << feat_point->getMeasurement() << "\nfeature covariance:\n" << feat_point->getMeasurementCovariance()<< std::endl;

//    delete feat_point;

    //ORB test
    std::cout << std::endl << " ========= ORB test ===========" << std::endl << std::endl;

    WolfProblem* wolf_problem_ = new WolfProblem();
    wolf_problem_->getHardwarePtr()->addSensor(sen_cam_);
    ProcessorORB* processor_ORB = new ProcessorORB();
    sen_cam_->addProcessor(processor_ORB);


    CaptureImage* cap_vid_orb_ptr;
    //CaptureImage* other_cap_vid_orb_ptr;

    if(!image_or_video)
    {
        wolf_problem_->getTrajectoryPtr()->addFrame(new FrameBase(TimeStamp(),new StateBlock(Eigen::Vector3s::Zero()), new StateBlock(Eigen::Vector3s::Zero())));
        wolf_problem_->getTrajectoryPtr()->getLastFramePtr()->addCapture(new CaptureImage(t,sen_cam_,image_cv,img_width,img_height));
        wolf_problem_->getTrajectoryPtr()->getLastFramePtr()->getCaptureListPtr()->back()->process();
    }
    else
    {
        const char * filename = "/home/datchuth/Public/WP_20160217_14_02_25_Pro.mp4" ;
        cv::VideoCapture capture(filename); //filename = 0 if canera
        cv::Mat frame;

        std::cout << "file opened: " << capture.isOpened() << std::endl;
        if( !capture.isOpened() )
        {
            throw "Error when reading steam_avi";
        }


        //cv::namedWindow("video");
        capture.set(CV_CAP_PROP_POS_MSEC, 3000);

        capture >> frame;
        cap_vid_orb_ptr = new CaptureImage(t,sen_cam_,frame,img_width,img_height);

        int f = 50;
        while(f<100)
        {
            capture >> frame;
            //cv::imshow("video",frame);
            wolf_problem_->getTrajectoryPtr()->addFrame(new FrameBase(TimeStamp(),new StateBlock(Eigen::Vector3s::Zero()), new StateBlock(Eigen::Vector3s::Zero())));

            //delete other_cap_vid_orb_ptr;
            //other_cap_vid_orb_ptr = cap_vid_orb_ptr;
            //CaptureImage* other_cap_vid_brisk = wolf_problem_->getTrajectoryPtr()->getLastFramePtr()->getCaptureListPtr()->back();

            cap_vid_orb_ptr = new CaptureImage(t,sen_cam_,frame,img_width,img_height);
            wolf_problem_->getTrajectoryPtr()->getLastFramePtr()->addCapture(cap_vid_orb_ptr);
            clock_t t1 = clock();
            processor_ORB->extractFeatures(cap_vid_orb_ptr);
            //processor_ORB->establishConstraints(other_cap_vid_orb_ptr); //
            //cap_vid_orb->process();
            std::cout << "Time: " << ((double) clock() - t1) / CLOCKS_PER_SEC << "s" << std::endl;
            f++;
        }
        //cv::waitKey(0);

    }

    wolf_problem_->destruct();
}