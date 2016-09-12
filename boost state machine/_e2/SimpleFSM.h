#pragma once

#pragma once
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30	// or whatever you need               
#define BOOST_MPL_LIMIT_MAP_SIZE 30		// or whatever you need 

#include <iostream>
#include <functional>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;

// boost의 meta state machine을 이용하여 Wolf의 상태를 정의하자.
// 상태기계를 정의하기 위한 기본적인 요소로는 state와 event(trigger)가 있다.
// State은 상태 그 자체를 의미하고,
// event(trigger)는 상태 전이를 위한 조건을 의미한다.
// 정확히는 전이 조건을 충족하는 경우 직접 발생시켜 주는 것이다. 

// boost meta state machine을 이용하여 간단한 cd player 상태 기계를 만들어 보자.

// 상태 : Stopped, Playing
// 상태 전이 : pressButton

namespace _e2
{
	// 상태 기계 정의
	struct SimpleFSM : public msm::front::state_machine_def<SimpleFSM>
	{
		// 상태 기계에 진입 시 호출
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&)
		{
			std::cout << "enter: [Simple FSM]" << std::endl;
		}

		// 상태 기계에서 나올 때 호출
		template <class Event, class FSM>
		void on_exit(Event const&, FSM&)
		{
			std::cout << "exit: [Simple FSM]" << std::endl;
		}

		// Events
		struct pressButton {};

		// States
		struct Playing : public msm::front::state<>
		{
			// 상태 진입 시 호출
			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [Play]" << std::endl; }

			// 상태에서 나갈 때 호출
			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [Play]" << std::endl; }
		};

		struct Stopped : public msm::front::state<>
		{
			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [Stopped]" << std::endl; }

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [Stopped]" << std::endl; }
		};

		// 초기 상태
		typedef Stopped initial_state;

		// 상태 전이 table
		struct transition_table
			:
			mpl::vector
			<
			//		  Start			Event			Next 	
			//+--------------------------------------------------------------------------------+
			_row	< Stopped,		pressButton,	Playing>,
			//+--------------------------------------------------------------------------------+
			_row	< Playing,		pressButton,	Stopped>
			//+--------------------------------------------------------------------------------+
			> {};
	};


	// Testing utilities.
	static char const* const state_names[] =
	{
		"Stopped", "Playing"
	};

	// 상태 기계의 current_state() 함수를 통해 현재 상태가 무엇인지 알 수 있다.
	// 상태에 0부터 번호가 매겨진다. 
	// 상태 전이 table의 Start 상태들 위에서부터 아래로
	// 상태 전이 table의 Next 상태들 위에서부터 아래로
	// 순으로 번호가 매겨진다.
	template<typename StateMachine>
	const char* toString(const StateMachine& s)
	{
		return state_names[s.current_state()[0]];
	}

	void test()
	{
		// 상태기계 생성
		msm::back::state_machine<SimpleFSM> fsm;

		// 상태기계 시작
		fsm.start();

		// 상태기계의 pressButton() event를 넣어줌으로써 전이를 일으킨다.
		fsm.process_event(SimpleFSM::pressButton());
		std::cout << "state : " << toString(fsm) << std::endl;

		fsm.process_event(SimpleFSM::pressButton());
		std::cout << "state : " << toString(fsm) << std::endl;

		// 상태기계 끝
		fsm.stop();
	}
}