#include "hardware_base.h"
#include "sensor_base.h"
#include "wolf_problem.h"

HardwareBase::HardwareBase() :
    NodeLinked(MID, "HARDWARE")
{
    //std::cout << "HardwareBase::HardwareBase(): " << __LINE__ << std::endl;
}

HardwareBase::~HardwareBase()
{
	//std::cout << "deleting HardwareBase " << nodeId() << std::endl;
}

void HardwareBase::addSensor(SensorBase* _sensor_ptr)
{
    //std::cout << "adding sensor..." << std::endl;
	addDownNode(_sensor_ptr);
    //std::cout << "added!" << std::endl;

    // Remove Frame State Blocks
    if (getTop() != nullptr)
    {
        if (_sensor_ptr->getPPtr() != nullptr)
            getTop()->addStateBlockPtr(_sensor_ptr->getPPtr());

        if (_sensor_ptr->getOPtr() != nullptr)
            getTop()->addStateBlockPtr(_sensor_ptr->getOPtr());
    }

}

void HardwareBase::removeSensor(SensorBase* _sensor_ptr)
{
    removeDownNode(_sensor_ptr->nodeId());
}
