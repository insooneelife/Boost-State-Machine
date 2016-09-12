#include "Wolf.h"

#include <iostream>
#include <string>
using namespace std;

// ����� Idle, GoHunt, MovePlace, Die ���� ���¸� ���´�.

// ��������ǥ
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
	//ȭ�� ����� ���� string
	string by_string[4] = { "Idle", "GoHunt", "MovePlace", "Die" };


	Wolf::Wolf()
		:
		_state(Idle),
		_satiety(20)
	{}

	void Wolf::successInHunt()
	{
		cout << "��� ����!" << endl;
		_satiety += 10;
		if (_state == GoHunt)
		{
			_state = Idle;
		}
	}

	void Wolf::failInHunt()
	{
		cout << "��� ����!" << endl;
		if (_state == GoHunt)
		{
			_state = MovePlace;
		}
	}

	void Wolf::hungry()
	{
		cout << "hungry" << endl;
		if (_state == Idle)
		{
			_state = GoHunt;
		}
		else if (_state == MovePlace)
		{
			_state = GoHunt;
		}
	}

	void Wolf::lazy()
	{
		cout << "lazy" << endl;
		if (_state == Idle)
		{
			//~
			_state = MovePlace;
		}

		else if (_state == MovePlace)
		{
			_state = Idle;
		}
	}

	void Wolf::starved()
	{
		cout << "starved" << endl;
		if (_state == Idle)
		{
			_state = Die;
		}

		else if (_state == GoHunt)
		{
			_state = Die;
		}

		else if (_state == MovePlace)
		{
			_state = Die;
		}
	}


	void Wolf::updateIdle()
	{
		if (isStarved())
			starved();

		else if (isHungry())
			hungry();
	}

	void Wolf::updateGoHunt()
	{
		if (isStarved())
			starved();

		int temp = rand() % 2;

		if (temp == 1)
			successInHunt();
		else
			failInHunt();
	}

	void Wolf::updateMovePlace()
	{
		if (isStarved())
			starved();

		else if (isHungry())
			hungry();

		else
			lazy();
	}

	void Wolf::updateDie()
	{}



	bool Wolf::isHungry() const
	{
		return _satiety < 10;
	}

	bool Wolf::isStarved() const
	{
		return _satiety < 0;
	}


	bool Wolf::life()
	{
		cout << "������ : " << _satiety << "    ���� : [" << by_string[_state] << "]" << endl;

		if (_state == Die)
			return false;

		_satiety--;

		if (_state == Idle)
		{
			updateIdle();
		}
		else if (_state == GoHunt)
		{
			updateGoHunt();
		}
		else if (_state == MovePlace)
		{
			updateMovePlace();
		}
		else if (_state == Die)
		{
			updateDie();
		}

		return true;
	}
}