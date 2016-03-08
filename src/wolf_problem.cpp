#include "wolf_problem.h"
#include "constraint_base.h"
#include "state_block.h"
#include "state_quaternion.h"
#include "hardware_base.h"
#include "trajectory_base.h"
#include "map_base.h"

WolfProblem::WolfProblem(FrameStructure _frame_structure) :
        NodeBase("WOLF_PROBLEM"), //
		location_(TOP),
        trajectory_ptr_(new TrajectoryBase(_frame_structure)),
		map_ptr_(new MapBase),
		hardware_ptr_(new HardwareBase)
{
    trajectory_ptr_->linkToUpperNode( this );
	map_ptr_->linkToUpperNode( this );
	hardware_ptr_->linkToUpperNode( this );
}

WolfProblem::~WolfProblem()
{
    //std::cout << "deleting wolf problem " << nodeId() << std::endl;
    state_block_add_list_.clear();
    covariances_.clear();
    state_block_update_list_.clear();
    state_block_remove_list_.clear();
    constraint_add_list_.clear();
    constraint_remove_list_.clear();

	trajectory_ptr_->destruct();
    map_ptr_->destruct();
    hardware_ptr_->destruct();
}

void WolfProblem::destruct()
{
    delete this;
}

void WolfProblem::addSensor(SensorBase* _sen_ptr)
{
    getHardwarePtr()->addSensor(_sen_ptr);
}

void WolfProblem::createFrame(FrameType _frame_type, const TimeStamp& _time_stamp)
{
    switch ( trajectory_ptr_->getFrameStructure() )
    {
        case PO_2D:
            {
                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(2),
                                                        new StateBlock(1)));
                break;
            }
        case PO_3D:
            {
                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(3),
                                                        new StateQuaternion));
                break;
            }
        case POV_3D:
            {
                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(3),
                                                        new StateQuaternion,
                                                        new StateBlock(3)));
                break;
            }
        default:
            {
                assert( "Unknown frame structure. Add appropriate frame structure to the switch statement.");
            }
    }
}

void WolfProblem::createFrame(FrameType _frame_type, const Eigen::VectorXs& _frame_state, const TimeStamp& _time_stamp)
{
    //std::cout << "creating new frame..." << std::endl;

    // ---------------------- CREATE NEW FRAME ---------------------
    // Create frame
    switch ( trajectory_ptr_->getFrameStructure() )
    {
        case PO_2D:
            {
                assert( _frame_state.size() == 3 && "Wrong state vector size");

                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(_frame_state.head(2)),
                                                        new StateBlock(_frame_state.tail(1))));
                break;
            }
        case PO_3D:
            {
                assert( _frame_state.size() == 7 && "Wrong state vector size");

                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(_frame_state.head(3)),
                                                        new StateQuaternion(_frame_state.tail(4))));
                break;
            }
        case POV_3D:
            {
                assert( _frame_state.size() == 10 && "Wrong state vector size");

                trajectory_ptr_->addFrame(new FrameBase(_frame_type,
                                                        _time_stamp,
                                                        new StateBlock(_frame_state.head(3)),
                                                        new StateQuaternion(_frame_state.segment<3>(4)),
                                                        new StateBlock(_frame_state.tail(3))));
                break;
            }
        default:
            {
                assert( "Unknown frame structure. Add appropriate frame structure to the switch statement.");
            }
    }
    //std::cout << "new frame created" << std::endl;
}

void WolfProblem::addLandmark(LandmarkBase* _lmk_ptr)
{
    getMapPtr()->addLandmark(_lmk_ptr);
}

void WolfProblem::addStateBlockPtr(StateBlock* _state_ptr)
{
    // add the state unit to the list
    state_block_ptr_list_.push_back(_state_ptr);

    // queue for solver manager
    state_block_add_list_.push_back(_state_ptr);
}

void WolfProblem::updateStateBlockPtr(StateBlock* _state_ptr)
{
    // queue for solver manager
    state_block_update_list_.push_back(_state_ptr);
}

void WolfProblem::removeStateBlockPtr(StateBlock* _state_ptr)
{
    // add the state unit to the list
    state_block_ptr_list_.remove(_state_ptr);

    // queue for solver manager
    state_block_remove_list_.push_back(_state_ptr->getPtr());
}

void WolfProblem::addConstraintPtr(ConstraintBase* _constraint_ptr)
{
    // queue for solver manager
    constraint_add_list_.push_back(_constraint_ptr);
}

void WolfProblem::removeConstraintPtr(ConstraintBase* _constraint_ptr)
{
    // queue for solver manager
    constraint_remove_list_.push_back(_constraint_ptr->nodeId());
}

void WolfProblem::clearCovariance()
{
    covariances_.clear();
}

void WolfProblem::addCovarianceBlock(StateBlock* _state1, StateBlock* _state2, const Eigen::MatrixXs& _cov)
{
    assert(_state1->getSize() == (unsigned int) _cov.rows() && "wrong covariance block size");
    assert(_state2->getSize() == (unsigned int) _cov.cols() && "wrong covariance block size");

    covariances_[std::pair<StateBlock*, StateBlock*>(_state1, _state2)] = _cov;
}

void WolfProblem::getCovarianceBlock(StateBlock* _state1, StateBlock* _state2, Eigen::MatrixXs& _cov_block)
{
    if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state1, _state2)) != covariances_.end())
        _cov_block = covariances_[std::pair<StateBlock*, StateBlock*>(_state1, _state2)];
    else if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state2, _state1)) != covariances_.end())
        _cov_block = covariances_[std::pair<StateBlock*, StateBlock*>(_state2, _state1)].transpose();
    else
        assert("asking for a covariance block not getted from the solver");
}

void WolfProblem::getCovarianceBlock(StateBlock* _state1, StateBlock* _state2, Eigen::MatrixXs& _cov, const int _row, const int _col)
{
    //std::cout << "entire cov " << std::endl << _cov << std::endl;
    //std::cout << "_row " << _row << std::endl;
    //std::cout << "_col " << _col << std::endl;
    //if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state1, _state2)) != covariances_.end())
    //    std::cout << "stored cov" << std::endl << covariances_[std::pair<StateBlock*, StateBlock*>(_state1, _state2)] << std::endl;
    //else if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state2, _state1)) != covariances_.end())
    //    std::cout << "stored cov" << std::endl << covariances_[std::pair<StateBlock*, StateBlock*>(_state2, _state1)].transpose() << std::endl;

    if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state1, _state2)) != covariances_.end())
        _cov.block(_row,_col,_state1->getSize(),_state2->getSize()) = covariances_[std::pair<StateBlock*, StateBlock*>(_state1, _state2)];
    else if (covariances_.find(std::pair<StateBlock*, StateBlock*>(_state2, _state1)) != covariances_.end())
        _cov.block(_row,_col,_state1->getSize(),_state2->getSize()) = covariances_[std::pair<StateBlock*, StateBlock*>(_state2, _state1)].transpose();
    else
        assert("asking for a covariance block not getted from the solver");
}

void WolfProblem::addMap(MapBase* _map_ptr)
{
    // TODO: not necessary but update map maybe..
    map_ptr_ = _map_ptr;
    map_ptr_->linkToUpperNode( this );
}

void WolfProblem::addTrajectory(TrajectoryBase* _trajectory_ptr)
{
    trajectory_ptr_ = _trajectory_ptr;
    trajectory_ptr_->linkToUpperNode( this );
}

FrameBase* WolfProblem::getLastFramePtr()
{
    return trajectory_ptr_->getLastFramePtr();
}

void WolfProblem::removeDownNode(const LowerNodePtr _ptr)
{
    //
}

