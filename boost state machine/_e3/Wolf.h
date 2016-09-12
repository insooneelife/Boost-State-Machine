#include "StateMachine.h"

// boost의 meta 상태기계를 이용하여 Wolf의 상태를 정의하자.

namespace _e3
{
	class Wolf
	{
	public:
		Wolf::Wolf() : _satiety(20)
		{
			_fsm.set_states(msm::back::states_ << Fsm<Wolf>::Idle(this));
			_fsm.set_states(msm::back::states_ << Fsm<Wolf>::Die(this));
			_fsm.set_states(msm::back::states_ << Fsm<Wolf>::MovePlace(this));
			_fsm.set_states(msm::back::states_ << Fsm<Wolf>::GoHunt(this));
		}

		bool life()
		{
			std::cout << "포만감 : " << _satiety
				<< " 상태 : [" << toString(_fsm) << "]" << std::endl;

			if (_fsm.current_state()[0] == 3)
				return false;

			_fsm.visit_current_states();
			_satiety--;

			return true;
		}

		void eventOnSuccessInHunt()
		{
			_satiety += 10;
		}


		msm::back::state_machine<Fsm<Wolf> >& getFSM()
		{
			return _fsm;
		}

		bool isHungry() const
		{
			return _satiety < 10;
		}

		bool isStarved() const
		{
			return _satiety < 0;
		}

	private:

		// 상태기계
		msm::back::state_machine<Fsm<Wolf> > _fsm;

		// 포만감
		int _satiety;
	};
}