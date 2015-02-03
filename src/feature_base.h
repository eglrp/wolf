#ifndef FEATURE_BASE_H_
#define FEATURE_BASE_H_

// Forward declarations for node templates
class CaptureBase;
class CorrespondenceBase;

//std includes

//Wolf includes
#include "wolf.h"
#include "time_stamp.h"
#include "node_linked.h"
#include "capture_base.h"
#include "correspondence_base.h"

//class FeatureBase
class FeatureBase : public NodeLinked<CaptureBase,CorrespondenceBase>
{
    protected:
        Eigen::VectorXs measurement_;
        
    public:
        /** \brief Constructor
         * 
         * \param _capt_ptr a shared pointer to the Capture up node
         * \param _dim_measurement the dimension of the measurement space
         * 
         */
        FeatureBase(const CaptureBaseShPtr& _capt_ptr, unsigned int _dim_measurement);

        virtual ~FeatureBase();
        
        void linkToCapture(const CaptureBaseShPtr& _capt_ptr);
        
        void addCorrespondence(CorrespondenceBaseShPtr& _co_ptr);

        const CaptureBasePtr getCapturePtr() const;
        
        const CorrespondenceBaseList & getCorrespondenceList() const;
        
        const Eigen::VectorXs * getMeasurement() const;
        
        void setMeasurement(const Eigen::VectorXs & _meas);
        
        /** \brief Generic interface to find correspondences
         * 
         * TBD
         * Generic interface to find correspondences between this feature and a map (static/slam) or a previous feature
         *
         **/
        virtual void findCorrespondences() = 0;
        
        /** \brief prints object's info
         * 
         * prints object's info
         * 
         **/
        virtual void printSelf(unsigned int _ntabs = 0, std::ostream& _ost = std::cout) const;
        
        
};
#endif