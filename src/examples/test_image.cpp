// Testing things for the 3D image odometry

//Wolf includes
#include "sensor_camera.h"
#include "capture_image.h"
#include "processor_image.h"

//#include "feature_point_image.h"
//#include "state_block.h"
//#include "state_quaternion.h"
//#include "factory.h"

// opencv
//#include "opencv2/calib3d/calib3d.hpp"

// general includes
//#include "unistd.h"
//#include <time.h>
//#include <uEye.h> //used to use the camera?

//std includes
#include <ctime>
#include <iostream>


int main(int argc, char** argv)
{
    using namespace wolf;

    /**
    if (argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 1)
    {
        std::cout << "Please call me with: [./test_image IS_VIDEO], where:" << std::endl;
        std::cout << "    IS_VIDEO is 0 for image and 1 for video" << std::endl;
        std::cout << "EXIT due to bad user input" << std::endl << std::endl;
        return -1;
    }

    // auxiliar variables
    bool image_or_video = (atoi(argv[1]) == 1);
    */

    //ProcessorBrisk test
    std::cout << std::endl << " ========= ProcessorImage test ===========" << std::endl << std::endl;

    cv::VideoCapture capture;
    unsigned int f = 0;
    const char * filename;
    if (argc == 1)
    {
        //filename = "/home/jtarraso/Vídeos/House interior.mp4";
        filename = "/home/jtarraso/Vídeos/gray.mp4";
        capture.open(filename);
    }
    else
    {
        if (std::string(argv[1]) == "0")
        {
            //camera
            filename = "0";
            capture.open(0);
        }
        else
        {
            filename = argv[1];
            capture.open(filename);
        }
    }
    std::cout << "Input video file: " << filename << std::endl;
    //cv::VideoCapture capture(filename);
    if(!capture.isOpened())  // check if we succeeded
    {
        std::cout << "failed" << std::endl;
    }
    else
    {
        std::cout << "succeded" << std::endl;
    }

    capture.set(CV_CAP_PROP_POS_MSEC, 3000);
    unsigned int img_width = (unsigned int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
    unsigned int img_height = (unsigned int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    std::cout << "Image size: " << img_width << "x" << img_height << std::endl;

    unsigned int buffer_size = 4;
    std::vector<cv::Mat> frame(buffer_size);

    TimeStamp t = 1;


    Problem* wolf_problem_ = new Problem(FRM_PO_3D);

    //=====================================================
    // Method 1: Use data generated here for sensor and processor
    //=====================================================

    //    // SENSOR
    //    Eigen::Vector4s k = {320,240,320,320};
    //    SensorCamera* sen_cam_ = new SensorCamera(new StateBlock(Eigen::Vector3s::Zero()),
    //                                              new StateBlock(Eigen::Vector3s::Zero()),
    //                                              new StateBlock(k,false),img_width,img_height);
    //
    //    wolf_problem_->getHardwarePtr()->addSensor(sen_cam_);
    //
    //    // PROCESSOR
    //    ProcessorImageParameters tracker_params;
    //    tracker_params.image = {img_width,  img_height};
    //    tracker_params.matcher.min_normalized_score = 0.75;
    //    tracker_params.matcher.similarity_norm = cv::NORM_HAMMING;
    //    tracker_params.matcher.roi_width = 30;
    //    tracker_params.matcher.roi_height = 30;
    //    tracker_params.active_search.grid_width = 12;
    //    tracker_params.active_search.grid_height = 8;
    //    tracker_params.active_search.separation = 1;
    //    tracker_params.algorithm.max_new_features =0;
    //    tracker_params.algorithm.min_features_for_keyframe = 20;
    //
    //    DetectorDescriptorParamsOrb orb_params;
    //    orb_params.type = DD_ORB;
    //
    //    DetectorDescriptorParamsBrisk brisk_params;
    //    brisk_params.type = DD_BRISK;
    //
    //    // select the kind of detector-descriptor parameters
    //    tracker_params.detector_descriptor_params_ptr = &orb_params; // choose ORB
    //
    //    ProcessorImage* prc_image = new ProcessorImage(tracker_params);
    //
    //    sen_cam_->addProcessor(prc_image);
    //=====================================================


    //=====================================================
    // Method 2: Use factory to create sensor and processor
    //=====================================================

    // SENSOR
    // one-liner API
    SensorBase* sensor_ptr = wolf_problem_->installSensor("CAMERA", "PinHole", Eigen::VectorXs::Zero(7), "/Users/jsola/dev/wolf/src/examples/camera.yaml");

    // PROCESSOR
    // one-liner API
    wolf_problem_->installProcessor("IMAGE", "ORB", "PinHole", "/Users/jsola/dev/wolf/src/examples/processor_image_ORB.yaml");
    // two-liner alternative with explicit access to pointers and params
    //    ProcessorParamsBase* prc_params = ProcessorParamsFactory::get().create("IMAGE", "/Users/jsola/dev/wolf/src/examples/processor_image_ORB.yaml");
    //    ProcessorBase* prc_ptr = wolf_problem_->installProcessor("IMAGE", "ORB", sensor_ptr, prc_params);
    //=====================================================


    // CAPTURES
    CaptureImage* capture_image_ptr;

    f  = 1;
    capture >> frame[f % buffer_size];

    cv::namedWindow("Feature tracker");    // Creates a window for display.
    cv::moveWindow("Feature tracker", 0, 0);

    while(!(frame[f % buffer_size].empty()))
    {
        std::cout << "\n=============== Frame #: " << f << " in buffer: " << f%buffer_size << " ===============" << std::endl;

        clock_t t1 = clock();

        // Old method with non-factory objects
        //        capture_image_ptr = new CaptureImage(t, sen_cam_,frame[f % buffer_size]);
        //        prc_image->process(capture_image_ptr);

        // Preferred method with factory objects: FIXME: not working yet
        capture_image_ptr = new CaptureImage(t, (SensorCamera*)sensor_ptr, frame[f % buffer_size]);
        capture_image_ptr->process();

        std::cout << "Time: " << ((double) clock() - t1) / CLOCKS_PER_SEC << "s" << std::endl;
        cv::waitKey(10);

        f++;
        capture >> frame[f % buffer_size];
    }

    wolf_problem_->destruct();
}
