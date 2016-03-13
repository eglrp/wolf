#ifndef PROCESSOR_BASE_H_
#define PROCESSOR_BASE_H_

// Fwd refs
class SensorBase;
class NodeTerminus;

//Wolf includes
#include "wolf.h"
#include "node_linked.h"

// Std includes

//class ProcessorBase
class ProcessorBase : public NodeLinked<SensorBase, NodeTerminus>
{
    public:
        ProcessorBase(ProcessorType _tp);

        /** \brief Default destructor (not recommended)
         *
         * Default destructor (please use destruct() instead of delete for guaranteeing the wolf tree integrity)
         *
         **/
        virtual ~ProcessorBase();

        SensorBase* getSensorPtr();

        virtual void process(CaptureBase* _capture_ptr) = 0;

    private:
        ProcessorType type_;

};

inline SensorBase* ProcessorBase::getSensorPtr()
{
    return upperNodePtr();
}

#endif
