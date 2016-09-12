#include "windows.h"
#include "time.h"
#include "stdlib.h"

#include "_e1/Wolf.h"

#include "_e2/SimpleFSM.h"

#include "_e3/StateMachine.h"
#include "_e3/Wolf.h"

#include "_e4/StateMachine.h"
#include "_e4/Wolf.h"

using namespace std;
template <class Wolf>
void test(Wolf& wolf)
{
	srand(static_cast<unsigned int>(time(NULL)));
	while (1)
	{
		if (!wolf.life())
			break;
		Sleep(300);
	}
}

int main()
{
	_e1::Wolf wolf1;
	_e3::Wolf wolf3;
	_e4::Wolf wolf4;

	//_e2::test();

	test(wolf1);
	return 0;
}
