#include "capture_gps.h"


CaptureGPS::CaptureGPS(const TimeStamp &_ts, SensorBase *_sensor_ptr, rawgpsutils::SatellitesObs &_obs) :
        CaptureBase(_ts, _sensor_ptr),
        obs_(_obs)
{
    std::cout << "CaptureGPS constructor.\t\tReceived " << obs_.toString() << " sats" << std::endl;
}


CaptureGPS::~CaptureGPS()
{
    //std::cout << "deleting CaptureGPS " << nodeId() << std::endl;
}


/*
 * Dummy implementation of the method, only because it's pure virtual
 */
Eigen::VectorXs CaptureGPS::computeFramePose(const TimeStamp &_now) const
{
    return Eigen::Vector3s(0, 0, 0);
}

rawgpsutils::SatellitesObs &CaptureGPS::getData()
{
    return obs_;
}
