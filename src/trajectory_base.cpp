#include "trajectory_base.h"
#include "frame_base.h"
#include "wolf_problem.h"


TrajectoryBase::TrajectoryBase(FrameStructure _frame_structure) :
    NodeLinked(MID, "TRAJECTORY"),
    frame_structure_(_frame_structure), fixed_size_(0)
{
    //
}

TrajectoryBase::~TrajectoryBase()
{
    //std::cout << "deleting TrajectoryBase " << nodeId() << std::endl;
}

void TrajectoryBase::addFrame(FrameBase* _frame_ptr)
{
	addDownNode(_frame_ptr);
    if (getTop() != nullptr)
    {
        if (_frame_ptr->getPPtr() != nullptr)
            getTop()->addStateBlockPtr(_frame_ptr->getPPtr());
        if (_frame_ptr->getOPtr() != nullptr)
            getTop()->addStateBlockPtr(_frame_ptr->getOPtr());
        if (_frame_ptr->getVPtr() != nullptr)
            getTop()->addStateBlockPtr(_frame_ptr->getVPtr());
    }
}

void TrajectoryBase::getConstraintList(ConstraintBaseList & _ctr_list)
{
	for(auto fr_it = getFrameListPtr()->begin(); fr_it != getFrameListPtr()->end(); ++fr_it)
		(*fr_it)->getConstraintList(_ctr_list);
}
