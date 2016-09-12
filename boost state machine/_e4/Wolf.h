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
			std::cout << "포만감 : " << _satiety
				<< " 상태 : [" << toString(_fsm) << "]" << std::endl;

			if (_fsm.current_state()[0] == 3)
				return false;

			// http://www.boost.org/doc/libs/1_61_0/libs/msm/doc/HTML/ch03s05.html#d0e2433
			// Visitor 파트의 Important warning: 부분 확인

			// 이 경우 reference가 전달되지 않는다.
			//_fsm.visit_current_states(*this);

			// 이 경우 reference가 정확히 전달된다.
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

		// 상태기계
		msm::back::state_machine<Fsm<Wolf> > _fsm;

		// 포만감
		int _satiety;
	};
}