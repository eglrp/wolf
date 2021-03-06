
#ifndef CAPTURE_ODOM_2D_H_
#define CAPTURE_ODOM_2D_H_

//Wolf includes
#include "capture_motion.h"
#include "feature_odom_2D.h"
#include "sensor_odom_2D.h"

//std includes
//

namespace wolf {

class CaptureOdom2D : public CaptureMotion
{

    public:

      CaptureOdom2D(const TimeStamp& _init_ts, const TimeStamp& _final_ts, SensorBase* _sensor_ptr, const Eigen::Vector3s& _data);

      CaptureOdom2D(const TimeStamp& _init_ts, const TimeStamp& _final_ts, SensorBase* _sensor_ptr, const Eigen::Vector3s& _data, const Eigen::Matrix3s& _data_covariance);

      /** \brief Default destructor (not recommended)
       *
       * Default destructor (please use destruct() instead of delete for guaranteeing the wolf tree integrity)
       *
       **/
      virtual ~CaptureOdom2D();

      virtual void process();

      virtual Eigen::VectorXs computeFramePose(const TimeStamp& _now = 0) const;

      virtual void addConstraints();

      virtual void integrateCapture(CaptureMotion* _new_capture);

      virtual CaptureOdom2D* interpolateCapture(const TimeStamp& _ts);

};

} // namespace wolf
#endif
