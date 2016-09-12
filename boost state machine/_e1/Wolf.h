
//	���� ���±��

//	����
//	Idle, GoHunt, MovePlace, Die 

//	���� ���� event
//	hungry, lazy, starved, successInHunt, failInHunt, 

//	��������ǥ
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


//if���� ���� flag�� �̿��Ͽ� ������ ����.

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

		// ���� ���� event
		void successInHunt();
		void failInHunt();
		void hungry();
		void lazy();
		void starved();

		// ���� update
		void updateIdle();
		void updateGoHunt();
		void updateMovePlace();
		void updateDie();

		//���� ����
		int _state;

		//������
		int _satiety;
	};
}

// �� ���谡 �ùٸ� �����ΰ�?
// 1. Ȯ�强�� ���� ������ ����.
// ���� ���� event�� ���� �ߺ��Ǵ� ��찡 ���� ��, ���������� �ʴ�.
// ���� ��� starved�� ��� ���� ���¿� ���� ���̰� �߻��� �� �ִ�.

// 2. ����� ���鿡�� �����غ���.
// ���뿡 ���� ���谡 �� �� ���������� ���°� 10��, ���� ���̰� 30���� �þ�ٰ� �����غ���.
// �׸��� � a��� ���¿��� ������ ����ٰ� ���� ��, ���� ������ �ذ��� �� ������?
// �ϳ��� ���¿��� ������ ������� �����ڴ� ��� ���� ���� �Լ��� �� �˻��ؾ� �Ѵ�.
// �׷��ٸ� �α��� ���� ���� �Լ��鿡 �Ѵٸ� ���� �ذ��� ��������?
// ���� ���� �Լ��鵵 �� ���¸��� ���� �ֱ� ������ �ᱹ ��� ���¸� �� �˻��ؾ� �Ѵ�.