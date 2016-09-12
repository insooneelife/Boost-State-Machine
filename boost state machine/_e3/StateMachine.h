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

// 문제 1.
// cd player와 달리 Wolf는 자기 자신이 각 상태에 대한 행위를 수행한다.
// 또한 스스로가 상태를 변경시킨다.
// 그러므로 각 상태에 맞게 처리를 하는 함수를 갖도록 구현해야 한다.
// 이 문제를 처리하기 위해 boost msm에서는 visitor를 제공한다.

// 문제 2. 
// 각 상태 자체가 어떤 작업을 수행하도록 하기 위해, 상태들은 자신의 주인을 알 필요가 있다.
// (만약 늑대의 "사냥"이라는 상태가 있다면 이 상태의 주인은 '늑대' 가 된다.)
// 그러므로 각 상태는 주인에 대한 reference 혹은 ponter를 갖고 있어야 한다.

namespace _e3
{
	// Visitor를 제공하기 위한 부모 class
	struct AbstState
	{
		// boost msm에서는 visitor 함수로 accept 함수를 제공해 주는데,
		// 이 함수의 sigature를 아래와 같이 정의한다.
		typedef msm::back::args<void> accept_sig;

		// 다형성을 적용시키고 적절한 제거를 위해..
		virtual ~AbstState() {}

		// visit이 필요없는 상태 class들을 위한 defalt 함수
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