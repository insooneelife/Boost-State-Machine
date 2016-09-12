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

namespace _e4
{
	// This one is for visitor
	template <class Entity>
	struct AbstState
	{
		// signature of the accept function
		typedef msm::back::args<void, Entity&> accept_sig;

		// we also want polymorphic states
		virtual ~AbstState() {}

		// default implementation for states who do not need to be visited
		void accept(Entity&) {}
	};

	// Needs to give AbstState in state machine too.
	template <class Entity>
	struct Fsm 
		:
		public msm::front::state_machine_def<Fsm<Entity>, AbstState<Entity> >
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
		template<class Entity>
		struct hungry 
		{
			hungry(Entity& entity) : owner(entity) {}
			Entity& owner;
		};

		template<class Entity>
		struct lazy 
		{
			lazy(Entity& entity) : owner(entity) {}
			Entity& owner;
		};

		template<class Entity>
		struct starved 
		{
			starved(Entity& entity) : owner(entity) {}
			Entity& owner;
		};

		template<class Entity>
		struct successInHunt 
		{
			successInHunt(Entity& entity) : owner(entity) {}
			Entity& owner;
		};

		template<class Entity>
		struct failInHunt 
		{
			failInHunt(Entity& entity) : owner(entity) {}
			Entity& owner;
		};

		// Transition actions
		void actionInHungry(hungry<Entity> const&) 
		{
			std::cout << "actionInHungry" << std::endl; 
		}

		// States
		// Inherits AbstState for visit
		struct Idle : public msm::front::state<AbstState<Entity> >
		{
			template <class Event, class FSM>
			void on_entry(Event const& evt, FSM&) 
			{
				std::cout << "enter: [Idle]" << std::endl; 
			}

			template <class Event, class FSM>
			void on_exit(Event const& evt, FSM&) { std::cout << "exit: [Idle]" << std::endl; }

			void accept(Entity& owner) 
			{
				if (owner.isStarved())
					owner.getFSM().process_event(starved<Entity>(owner));

				else if (owner.isHungry())
					owner.getFSM().process_event(hungry<Entity>(owner));
			}
		};

		struct GoHunt : public msm::front::state<AbstState<Entity> >
		{
			template <class Event, class FSM>
			void on_entry(Event const& evt, FSM&) 
			{
				std::cout << "enter: [GoHunt]" << std::endl; 
			}

			template <class Event, class FSM>
			void on_exit(Event const& evt, FSM&) 
			{
				std::cout << "exit: [GoHunt]" << std::endl; 
			}

			void accept(Entity& owner) 
			{
				if (owner.isStarved())
					owner.getFSM().process_event(starved<Entity>(owner));

				int temp = rand() % 2;

				if (temp == 1)
				{
					owner.getFSM().process_event(successInHunt<Entity>(owner));
					owner.eventOnSuccessInHunt();
				}
				else 
				{
					owner.getFSM().process_event(failInHunt<Entity>(owner));
				}
			}
		};

		struct MovePlace : public msm::front::state<AbstState<Entity> >
		{
			template <class Event, class FSM>
			void on_entry(Event const& evt, FSM&) 
			{
				std::cout << "enter: [MovePlace]" << std::endl; 
			}

			template <class Event, class FSM>
			void on_exit(Event const& evt, FSM&) 
			{
				std::cout << "exit: [MovePlace]" << std::endl; 
			}

			void accept(Entity& owner) 
			{
				if (owner.isStarved())
					owner.getFSM().process_event(starved<Entity>(owner));

				else if (owner.isHungry())
					owner.getFSM().process_event(hungry<Entity>(owner));

				else
					owner.getFSM().process_event(lazy<Entity>(owner));
			}
		};

		struct Die : public msm::front::state<AbstState<Entity> >
		{
			template <class Event, class FSM>
			void on_entry(Event const& evt, FSM&) 
			{
				std::cout << "enter: [Die]" << std::endl; 
			}

			template <class Event, class FSM>
			void on_exit(Event const& evt, FSM&) 
			{
				std::cout << "exit: [Die]" << std::endl; 
			}

			void accept(Entity& owner) 
			{
				std::cout << "Wolf is dead!!" << std::endl;
			}
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
			a_row	< Idle, hungry<Entity>, GoHunt, &Fsm::actionInHungry>,
			_row	< Idle, lazy<Entity>, MovePlace>,
			_row	< Idle, starved<Entity>, Die>,
			//+--------------------------------------------------------------------------------+
			_row	< GoHunt, starved<Entity>, Die>,
			_row	< GoHunt, successInHunt<Entity>, Idle>,
			_row	< GoHunt, failInHunt<Entity>, MovePlace >,
			//+--------------------------------------------------------------------------------+
			_row	< MovePlace, hungry<Entity>, GoHunt>,
			_row	< MovePlace, lazy<Entity>, Idle >,
			_row	< MovePlace, starved<Entity>, Die >
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