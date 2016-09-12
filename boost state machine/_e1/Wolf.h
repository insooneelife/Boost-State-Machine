
//	늑대 상태기계

//	상태
//	Idle, GoHunt, MovePlace, Die 

//	상태 전이 event
//	hungry, lazy, starved, successInHunt, failInHunt, 

//	상태전이표
//
//	Idle		hungry			GoHunt	
//	Idle		lazy			MovePlace
//	Idle		starved			Die
//	GoHunt		starved			Die
//	GoHunt		successInHunt	Idle
//	GoHunt		failInHunt		MovePlace
//	MovePlace	hungry			GoHunt
//	MovePlace	lazy			Idle
//	MovePlace	starved			Die


//if문과 상태 flag를 이용하여 구현해 보자.

namespace _e1
{
	class Wolf
	{
	public:
		Wolf();
		bool life();

	private:
		enum
		{
			Idle, GoHunt, MovePlace, Die
		};

		bool isHungry() const;
		bool isStarved() const;

		// 상태 전이 event
		void successInHunt();
		void failInHunt();
		void hungry();
		void lazy();
		void starved();

		// 상태 update
		void updateIdle();
		void updateGoHunt();
		void updateMovePlace();
		void updateDie();

		//현재 상태
		int _state;

		//포만감
		int _satiety;
	};
}

// 이 설계가 올바른 설계인가?
// 1. 확장성에 대해 생각해 보자.
// 상태 전이 event에 대해 중복되는 경우가 생길 때, 직관적이지 않다.
// 예를 들어 starved의 경우 여러 상태에 대해 전이가 발생될 수 있다.

// 2. 디버깅 측면에서 생각해보자.
// 늑대에 대한 설계가 좀 더 복잡해져서 상태가 10개, 상태 전이가 30개로 늘어났다고 가정해보자.
// 그리고 어떤 a라는 상태에서 문제가 생겼다고 했을 때, 쉽게 문제를 해결할 수 있을까?
// 하나의 상태에서 문제가 생기더라도 개발자는 모든 상태 전이 함수를 다 검사해야 한다.
// 그렇다면 로깅을 상태 전이 함수들에 한다면 문제 해결이 쉬워질까?
// 상태 전이 함수들도 각 상태마다 섞여 있기 때문에 결국 모든 상태를 다 검사해야 한다.