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

// boost�� meta state machine�� �̿��Ͽ� Wolf�� ���¸� ��������.
// ���±�踦 �����ϱ� ���� �⺻���� ��ҷδ� state�� event(trigger)�� �ִ�.
// State�� ���� �� ��ü�� �ǹ��ϰ�,
// event(trigger)�� ���� ���̸� ���� ������ �ǹ��Ѵ�.
// ��Ȯ���� ���� ������ �����ϴ� ��� ���� �߻����� �ִ� ���̴�. 

// boost meta state machine�� �̿��Ͽ� ������ cd player ���� ��踦 ����� ����.

// ���� : Stopped, Playing
// ���� ���� : pressButton

namespace _e2
{
	// ���� ��� ����
	struct SimpleFSM : public msm::front::state_machine_def<SimpleFSM>
	{
		// ���� ��迡 ���� �� ȣ��
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&)
		{
			std::cout << "enter: [Simple FSM]" << std::endl;
		}

		// ���� ��迡�� ���� �� ȣ��
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
			// ���� ���� �� ȣ��
			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [Play]" << std::endl; }

			// ���¿��� ���� �� ȣ��
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

		// �ʱ� ����
		typedef Stopped initial_state;

		// ���� ���� table
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

	// ���� ����� current_state() �Լ��� ���� ���� ���°� �������� �� �� �ִ�.
	// ���¿� 0���� ��ȣ�� �Ű�����. 
	// ���� ���� table�� Start ���µ� ���������� �Ʒ���
	// ���� ���� table�� Next ���µ� ���������� �Ʒ���
	// ������ ��ȣ�� �Ű�����.
	template<typename StateMachine>
	const char* toString(const StateMachine& s)
	{
		return state_names[s.current_state()[0]];
	}

	void test()
	{
		// ���±�� ����
		msm::back::state_machine<SimpleFSM> fsm;

		// ���±�� ����
		fsm.start();

		// ���±���� pressButton() event�� �־������ν� ���̸� ����Ų��.
		fsm.process_event(SimpleFSM::pressButton());
		std::cout << "state : " << toString(fsm) << std::endl;

		fsm.process_event(SimpleFSM::pressButton());
		std::cout << "state : " << toString(fsm) << std::endl;

		// ���±�� ��
		fsm.stop();
	}
}