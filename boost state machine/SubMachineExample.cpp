#include <iostream>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30	// or whatever you need               
#define BOOST_MPL_LIMIT_MAP_SIZE 30		// or whatever you need 

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>

#include <functional>


namespace msm = boost::msm;
namespace mpl = boost::mpl;


// This one is for visitor
struct AbstState
{
	// signature of the accept function
	typedef msm::back::args<void> accept_sig;

	// we also want polymorphic states
	virtual ~AbstState() {}

	// default implementation for states who do not need to be visited
	void accept() {}
};

// Needs to give AbstState in state machine too.
struct AI : public msm::front::state_machine_def<AI, AbstState>
{
	template <class Event, class FSM>
	void on_entry(Event const&, FSM&)
	{
		std::cout << "enter: [AI FSM]" << std::endl;
	}
	template <class Event, class FSM>
	void on_exit(Event const&, FSM&)
	{
		std::cout << "exit: [AI FSM]" << std::endl;
	}
	
	// Events
	struct inState {};
	struct hasDestination {};
	struct arrive {};
	struct enemyInView {};
	struct enemyOutView {};
	struct enemyInRange {};
	struct readyToAttack {};
	struct doneAttack {};
	struct enemyOutRange {};
	struct hasToDie {};
	struct hasToDead {};
	struct followPath {};
	struct foundPath {};

	// States
	// Inherits AbstState for visit
	struct Patrol : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [Patrol]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [Patrol]" << std::endl; }

		void accept() { std::cout << "accept: [Patrol]" << std::endl; }
	};

	struct AttackToDestination : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [AttackToDestination]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [AttackToDestination]" << std::endl; }
		                                           
		void accept() { std::cout << "accept: [AttackToDestination]" << std::endl; }
	};

	struct ChaseEnemy : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [ChaseEnemy]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [ChaseEnemy]" << std::endl; }

		void accept() { std::cout << "accept: [ChaseEnemy]" << std::endl; }
	};

	struct Attack : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [Attack]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [Attack]" << std::endl; }

		void accept() { std::cout << "accept: [Attack]" << std::endl; }
	};

	struct WaitForNextAttack : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [WaitForNextAttack]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [WaitForNextAttack]" << std::endl; }
		
		void accept() { std::cout << "accept: [WaitForNextAttack]" << std::endl; }
	};

	struct Dying : public msm::front::state<AbstState>
	{	
		Dying(): _a(0), _b(0) {}
		Dying(int a, int b) : _a(a), _b(0) {}

		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [Dying]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [Dying]" << std::endl; }

		void accept() { std::cout << "accept: [Dying]" << std::endl; }

	private:
		int _a, _b;
	};

	struct Dead : public msm::front::state<AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&) { std::cout << "enter: [Dead]" << std::endl; }

		template <class Event, class FSM>
		void on_exit(Event const&, FSM&) { std::cout << "exit: [Dead]" << std::endl; }

		void accept() {  std::cout << "accept: [Dead]" << std::endl; }
	};

	// Sub-Statemachine
	struct FollowPath_ : public msm::front::state_machine_def<FollowPath_, AbstState>
	{
		template <class Event, class FSM>
		void on_entry(Event const&, FSM&)
		{
			std::cout << "enter: [FollowPath FSM]" << std::endl;
		}
		template <class Event, class FSM>
		void on_exit(Event const&, FSM&)
		{
			std::cout << "exit: [FollowPath FSM]" << std::endl;
		}

		// Events
		struct edgeRemains {};
		struct edgeTraversed {};

		struct CheckArrive : public msm::front::state<AbstState>
		{
			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) {}

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) {}

			void accept() { std::cout << "accept: [CheckArrive]" << std::endl; }
		};

		struct TraverseEdge : public msm::front::state<AbstState>
		{
			template <class Event, class FSM>
			void on_entry(Event const&, FSM&) {}

			template <class Event, class FSM>
			void on_exit(Event const&, FSM&) {}

			void accept() { std::cout << "accept: [TraverseEdge]" << std::endl; }
		};

		// Initial state
		typedef CheckArrive initial_state;

		// Transition table for player
		struct transition_table
			:
			mpl::vector
			<
			//		  Start					Event				Next 	
			//+--------------------------------------------------------------------------------+
			_row	< CheckArrive, edgeRemains, TraverseEdge>,
			//+--------------------------------------------------------------------------------+
			_row	< TraverseEdge, edgeTraversed, CheckArrive>
			//+--------------------------------------------------------------------------------+
			> {};
	};
	typedef msm::back::state_machine<FollowPath_> FollowPath;



	// Initial state
	typedef Patrol initial_state;

	// Transition table for player
	struct transition_table 
		:
		mpl::vector
		<
		//		  Start					Event				Next 	
		//+--------------------------------------------------------------------------------+
		_row	< Patrol,				inState,			Patrol>,
		_row	< Patrol,				hasDestination,		AttackToDestination>,
		_row	< Patrol,				enemyInView,		ChaseEnemy>,
		_row	< Patrol,				hasToDie,			Dying>,
		//+--------------------------------------------------------------------------------+
		_row	< AttackToDestination,	inState,			AttackToDestination>,
		_row	< AttackToDestination,	arrive,				Patrol >,
		_row	< AttackToDestination,	enemyInView,		ChaseEnemy >,
		_row	< AttackToDestination,	hasToDie,			Dying>,
		//+--------------------------------------------------------------------------------+
		_row	< ChaseEnemy,			inState,			ChaseEnemy>,
		_row	< ChaseEnemy,			enemyOutView,		AttackToDestination >,
		_row	< ChaseEnemy,			enemyInRange,		Attack >,
		_row	< ChaseEnemy,			hasToDie,			Dying>,
		_row	< ChaseEnemy,			followPath,			FollowPath>,
		//+--------------------------------------------------------------------------------+
		_row	< FollowPath,			foundPath,			ChaseEnemy>,
		//+--------------------------------------------------------------------------------+
		_row	< Attack,				inState,			Attack>,
		_row	< Attack,				doneAttack,			WaitForNextAttack >,
		_row	< Attack,				hasToDie,			Dying>,
		//+--------------------------------------------------------------------------------+
		_row	< WaitForNextAttack,	inState,			WaitForNextAttack>,
		_row	< WaitForNextAttack,	enemyOutRange,		Patrol >,
		_row	< WaitForNextAttack,	readyToAttack,		Attack >,
		_row	< WaitForNextAttack,	hasToDie,			Dying>,
		//+--------------------------------------------------------------------------------+
		_row	< Dying,				inState,			Dying>,
		_row	< Dying,				hasToDead,			Dead >
		//+--------------------------------------------------------------------------------+
		> {};
};





void test(int a, int b)
{
	msm::back::state_machine<AI> ai;

	// For constructors with not default.
	ai.set_states(msm::back::states_ << AI::Dying(a, b));
	ai.start();

	ai.process_event(AI::hasDestination());
	ai.visit_current_states();

	ai.process_event(AI::arrive());
	ai.visit_current_states();

	ai.process_event(AI::enemyInView());
	ai.visit_current_states();

	ai.process_event(AI::followPath());
	ai.visit_current_states();

	ai.process_event(AI::FollowPath::edgeRemains());
	ai.visit_current_states();

	ai.process_event(AI::FollowPath::edgeTraversed());
	ai.visit_current_states();

	ai.process_event(AI::foundPath());
	ai.visit_current_states();

	ai.process_event(AI::hasToDie());
	ai.visit_current_states();

	// Index number is assigned by the order of defining transition table.
	// Ex) 
	// current_state()[0] = Patrol
	// current_state()[1] = AttackToDestination
	// current_state()[2] = ChaseEnemy
	

	ai.stop();
}


int main()
{
	test(1, 2);
	//test(3, 4);

	return 0;
}