/**
 * \file test_motion.cpp
 *
 *  Created on: Mar 15, 2016
 *      \author: jsola
 */

// Classes under test
#include "processor_odom_2D.h"

// Wolf includes
#include "state_block.h"
#include "wolf.h"

// STL includes
#include <map>
#include <list>
#include <algorithm>
#include <iterator>

// General includes
#include <iostream>
#include <iomanip>      // std::setprecision

int main()
{
    std::cout << std::setprecision(3);

    using namespace wolf;

    // time
    TimeStamp t0, t;
    t0.setToNow();
    t = t0;
    Scalar dt = .01;

    // Origin frame:
    Eigen::Vector2s p0;
    p0 << 0.5, -0.5 - sqrt(0.5);
    Eigen::Vector1s o0(Eigen::Vector1s::Constant(Constants::PI / 4));
    Eigen::Vector3s x0;
    x0 << p0, o0;

//    // robot state blocks: origin
//    StateBlock robot_p_(Eigen::Vector2s::Zero());
//    StateBlock robot_o_(Eigen::Vector1s::Zero());

// motion data
    Eigen::VectorXs data(2);
    data << 1, Constants::PI / 4;  // advance 1m turn 0.1 rad (aprox 6 deg)
    Eigen::MatrixXs data_cov = Eigen::MatrixXs::Identity(2, 2) * 0.01;

    // Create Wolf tree nodes
    Problem* problem_ptr = new Problem(FRM_PO_2D);
    SensorBase* sensor_ptr = new SensorBase(SEN_ODOM_2D, new StateBlock(Eigen::Vector2s::Zero()),
                                            new StateBlock(Eigen::Vector1s::Zero()),
                                            new StateBlock(Eigen::VectorXs::Zero(0)), 0);
    ProcessorOdom2d* odom2d_ptr = new ProcessorOdom2d();
    // Assemble Wolf tree by linking the nodes
    sensor_ptr->addProcessor(odom2d_ptr);
    problem_ptr->addSensor(sensor_ptr);

    // Initialize
    odom2d_ptr->setOrigin(x0, t0);

    std::cout << "Initial pose : " << problem_ptr->getLastFramePtr()->getState().transpose() << std::endl;
    std::cout << "Motion data  : " << data.transpose() << std::endl;

    std::cout << "\nIntegrating states at synchronous time values..." << std::endl;

    std::cout << "State(" << (t - t0) << ") : " << odom2d_ptr->getState().transpose() << std::endl;
    // Capture to use as container for all incoming data
    t += dt;
    CaptureMotion2* cap_ptr = new CaptureMotion2(t, sensor_ptr, data, data_cov);

    // Check covariance values
    Eigen::Vector3s integrated_x = x0;
    Eigen::Vector3s integrated_delta = Eigen::Vector3s::Zero();
    Eigen::Matrix3s integrated_covariance = Eigen::Matrix3s::Zero();
    Eigen::MatrixXs Ju(3, 2);
    Eigen::Matrix3s Jx = Eigen::Matrix3s::Identity();

    for (int i = 0; i <= 8; i++)
    {
        // Processor
        odom2d_ptr->process(cap_ptr);
        //std::cout << "----------- PROCESSOR:" << std::endl;
        //std::cout << "State(" << (t - t0) << ") : " << odom2d_ptr->getState().transpose() << std::endl;
        //std::cout << "Covariance(" << (t - t0) << ") : " << std::endl
        //        << odom2d_ptr->getBufferPtr()->get().back().delta_integr_cov_ << std::endl;

        // Reference
        Ju(0, 0) = cos(integrated_delta(2) + data(1) / 2);
        Ju(1, 0) = sin(integrated_delta(2) + data(1) / 2);
        Ju(2, 0) = 0;
        Ju(0, 1) = -data(0) / 2 * sin(integrated_delta(2) + data(1) / 2);
        Ju(1, 1) = data(0) / 2 * cos(integrated_delta(2) + data(1) / 2);
        Ju(2, 1) = 1;

        Jx(0, 2) = -sin(integrated_delta(2) + data(1) / 2) * data(0);
        Jx(1, 2) = cos(integrated_delta(2) + data(1) / 2) * data(0);

        integrated_covariance = Jx * integrated_covariance * Jx.transpose() + Ju * data_cov * Ju.transpose();

        integrated_delta(0) = integrated_delta(0) + cos(integrated_delta(2) + data(1) / 2) * data(0);
        integrated_delta(1) = integrated_delta(1) + sin(integrated_delta(2) + data(1) / 2) * data(0);
        integrated_delta(2) = integrated_delta(2) + data(1);

        integrated_x(0) = integrated_x(0) + cos(integrated_x(2) + data(1) / 2) * data(0);
        integrated_x(1) = integrated_x(1) + sin(integrated_x(2) + data(1) / 2) * data(0);
        integrated_x(2) = integrated_x(2) + data(1);

        //std::cout << "REFERENCE:" << std::endl;
        //std::cout << "State(" << (t - t0) << ") : " << integrated_x.transpose()
        //        << std::endl;
        //std::cout << "Covariance(" << (t - t0) << ") : " << std::endl
        //        << integrated_covariance << std::endl;

        // CHeck
        //std::cout << "ERROR:" << std::endl;
        //std::cout << "State error(" << (t - t0) << ") : " << odom2d_ptr->getState().transpose() - integrated_x.transpose()
        //        << std::endl;
        //std::cout << "Covariance error(" << (t - t0) << ") : " << std::endl
        //        << odom2d_ptr->getBufferPtr()->get().back().delta_integr_cov_ - integrated_covariance << std::endl;
        //std::cout << "Max coeff: " << std::endl
        //        << (odom2d_ptr->getBufferPtr()->get().back().delta_integr_cov_ - integrated_covariance).maxCoeff()
        //        << std::endl;

        if ((odom2d_ptr->getState() - integrated_x).norm() > 1e-12)
            throw std::runtime_error("Integrated state different from reference.");

        if ((odom2d_ptr->getBufferPtr()->get().back().delta_integr_cov_ - integrated_covariance).maxCoeff() > 1e-12)
            throw std::runtime_error("Integrated covariance different from reference.");

        // Timestamp
        t += dt;
        cap_ptr->setTimeStamp(t);
    }

    std::cout << "\nQuery states at asynchronous time values..." << std::endl;

    t = t0;
    dt = 0.0045; // new dt
    for (int i = 1; i <= 25; i++)
    {
        std::cout << "State(" << (t - t0) << ") = " << odom2d_ptr->getState(t/*+dt/2*/).transpose() << std::endl;
        //std::cout << "Covariance(" << (t - t0) << ") : " << std::endl
        //        << odom2d_ptr->getBufferPtr()->getMotion(t/*+dt/2*/).delta_integr_cov_ << std::endl;
        t += dt;
    }
    std::cout << "       ^^^^^^^   After the last time-stamp the buffer keeps returning the last member." << std::endl;

    // Split the buffer

    std::cout << "\nSplitting the buffer!\n---------------------" << std::endl;
    std::cout << "Original buffer:           < ";
    for (const auto &s : odom2d_ptr->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << ">" << std::endl;

    // first split at non-exact timestamp
    TimeStamp t_split = t0 + 0.033;
    std::cout << "Split time:                  " << t_split - t0 << std::endl;

    FrameBase* new_keyframe_ptr = problem_ptr->createFrame(KEY_FRAME, odom2d_ptr->getState(t_split), t_split);

    odom2d_ptr->keyFrameCallback(new_keyframe_ptr);

    std::cout << "New buffer: oldest part:   < ";
    for (const auto &s : ((CaptureMotion2*)(new_keyframe_ptr->getCaptureListPtr()->front()))->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << ">" << std::endl;

    std::cout << "Original keeps the newest: < ";
    for (const auto &s : odom2d_ptr->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << ">" << std::endl;

    std::cout << "All in one row:            < ";
    for (const auto &s : ((CaptureMotion2*)(new_keyframe_ptr->getCaptureListPtr()->front()))->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << "> " << t_split - t0 << " < ";
    for (const auto &s : odom2d_ptr->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << ">" << std::endl;

    // second split as exact timestamp

    t_split = t0 + 0.07;
    std::cout << "New split time:              " << t_split - t0 << std::endl;

    new_keyframe_ptr = problem_ptr->createFrame(KEY_FRAME, odom2d_ptr->getState(t_split), t_split);
    odom2d_ptr->keyFrameCallback(new_keyframe_ptr);

    std::cout << "All in one row:            < ";
    for (const auto &s : ((CaptureMotion2*)(new_keyframe_ptr->getCaptureListPtr()->front()))->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << "> " << t_split - t0 << " < ";
    for (const auto &s : odom2d_ptr->getBufferPtr()->get())
        std::cout << s.ts_ - t0 << ' ';
    std::cout << ">" << std::endl;

    // Free allocated memory
    cap_ptr->destruct();

    return 0;
}
