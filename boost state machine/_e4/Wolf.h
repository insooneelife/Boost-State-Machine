#include "StateMachine.h"

namespace _e4
{
	class Wolf
	{
	public:
		Wolf::Wolf() : _satiety(20)
		{}

		bool life()
		{
			std::cout << "������ : " << _satiety
				<< " ���� : [" << toString(_fsm) << "]" << std::endl;

			if (_fsm.current_state()[0] == 3)
				return false;

			// http://www.boost.org/doc/libs/1_61_0/libs/msm/doc/HTML/ch03s05.html#d0e2433
			// Visitor ��Ʈ�� Important warning: �κ� Ȯ��

			// �� ��� reference�� ���޵��� �ʴ´�.
			//_fsm.visit_current_states(*this);

			// �� ��� reference�� ��Ȯ�� ���޵ȴ�.
			_fsm.visit_current_states(boost::ref(*this));
			_satiety--;

			return true;
		}

		void eventOnSuccessInHunt()
		{
			std::cout << "eventOnSuccessInHunt()";
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