#include "StateMachine.h"

// boost�� meta ���±�踦 �̿��Ͽ� Wolf�� ���¸� ��������.

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
			std::cout << "������ : " << _satiety
				<< " ���� : [" << toString(_fsm) << "]" << std::endl;

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

		// ���±��
		msm::back::state_machine<Fsm<Wolf> > _fsm;

		// ������
		int _satiety;
	};
}