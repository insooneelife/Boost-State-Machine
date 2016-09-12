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

// ���� 1.
// cd player�� �޸� Wolf�� �ڱ� �ڽ��� �� ���¿� ���� ������ �����Ѵ�.
// ���� �����ΰ� ���¸� �����Ų��.
// �׷��Ƿ� �� ���¿� �°� ó���� �ϴ� �Լ��� ������ �����ؾ� �Ѵ�.
// �� ������ ó���ϱ� ���� boost msm������ visitor�� �����Ѵ�.

// ���� 2. 
// �� ���� ��ü�� � �۾��� �����ϵ��� �ϱ� ����, ���µ��� �ڽ��� ������ �� �ʿ䰡 �ִ�.
// (���� ������ "���"�̶�� ���°� �ִٸ� �� ������ ������ '����' �� �ȴ�.)
// �׷��Ƿ� �� ���´� ���ο� ���� reference Ȥ�� ponter�� ���� �־�� �Ѵ�.

namespace _e3
{
	// Visitor�� �����ϱ� ���� �θ� class
	struct AbstState
	{
		// boost msm������ visitor �Լ��� accept �Լ��� ������ �ִµ�,
		// �� �Լ��� sigature�� �Ʒ��� ���� �����Ѵ�.
		typedef msm::back::args<void> accept_sig;

		// �������� �����Ű�� ������ ���Ÿ� ����..
		virtual ~AbstState() {}

		// visit�� �ʿ���� ���� class���� ���� defalt �Լ�
		void accept() {}
	};

	// Needs to give AbstState in state machine too.
	template <class Entity>
	struct Fsm : public msm::front::state_machine_def<Fsm<Entity>, AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&)
		{
			std::cout << "enter: [Wolf FSM]" << std::endl;
		}
		template <class Event, class FSM>
		void on_exit(Event const&, FSM&)
		{
			std::cout << "exit: [Wolf FSM]" << std::endl;
		}

		// Events
		struct hungry {};
		struct lazy {};
		struct starved {};
		struct successInHunt {};
		struct failInHunt {};

		// States
		// Inherits AbstState for visit
		struct Idle : public msm::front::state<AbstState>
		{
			Idle() : _owner(nullptr) {}
			Idle(Entity* owner) : _owner(owner) {}

			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [Idle]" << std::endl; }

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [Idle]" << std::endl; }

			void accept() 
			{
				if (_owner->isStarved())
					_owner->getFSM().process_event(starved());

				else if (_owner->isHungry())
					_owner->getFSM().process_event(hungry());
			}

		private:
			Entity* _owner;
		};

		struct GoHunt : public msm::front::state<AbstState>
		{
			GoHunt() : _owner(nullptr) {}
			GoHunt(Entity* owner) : _owner(owner) {}

			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [GoHunt]" << std::endl; }

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [GoHunt]" << std::endl; }

			void accept() 
			{
				if (_owner->isStarved())
					_owner->getFSM().process_event(starved());

				int temp = rand() % 2;

				if (temp == 1)
				{
					_owner->getFSM().process_event(successInHunt());
					_owner->eventOnSuccessInHunt();
				}
				else 
				{
					_owner->getFSM().process_event(failInHunt());
				}
			}

		private:
			Entity* _owner;
		};

		struct MovePlace : public msm::front::state<AbstState>
		{
			MovePlace() : _owner(nullptr) {}
			MovePlace(Entity* owner) : _owner(owner) {}

			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [MovePlace]" << std::endl; }

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [MovePlace]" << std::endl; }

			void accept() 
			{
				if (_owner->isStarved())
					_owner->getFSM().process_event(starved());

				else if (_owner->isHungry())
					_owner->getFSM().process_event(hungry());

				else
					_owner->getFSM().process_event(lazy());
			}

		private:
			Entity* _owner;
		};

		struct Die : public msm::front::state<AbstState>
		{
			Die() : _owner(nullptr) {}
			Die(Entity* owner) : _owner(owner) {}

			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) { std::cout << "enter: [Die]" << std::endl; }

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) { std::cout << "exit: [Die]" << std::endl; }

			void accept() 
			{
				std::cout << "Wolf is dead!!" << std::endl;
			}

		private:
			Entity* _owner;
		};

		// Initial state
		typedef Idle initial_state;

		// Transition table for player
		struct transition_table
			:
			mpl::vector
			<
			//		  Start					Event				Next 	
			//+--------------------------------------------------------------------------------+
			_row	< Idle, hungry, GoHunt>,
			_row	< Idle, lazy, MovePlace>,
			_row	< Idle, starved, Die>,
			//+--------------------------------------------------------------------------------+
			_row	< GoHunt, starved, Die>,
			_row	< GoHunt, successInHunt, Idle>,
			_row	< GoHunt, failInHunt, MovePlace >,
			//+--------------------------------------------------------------------------------+
			_row	< MovePlace, hungry, GoHunt>,
			_row	< MovePlace, lazy, Idle >,
			_row	< MovePlace, starved, Die >
			//+--------------------------------------------------------------------------------+
			> {};
	};

	
	// Testing utilities.
	static char const* const state_names[] = 
	{
		"Idle", "GoHunt", "MovePlace", "Die"
	};

	template<typename T>
	const char* toString(const T& s)
	{
		return state_names[s.current_state()[0]];
	}
}