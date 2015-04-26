#pragma once

#include "FSMState.h"

#define END_STATE_TABLE {nullptr, 0, nullptr, false}

namespace ADBLib
{
	class FSMTransition
	{
	public:
		FSMState* currentState;
		int input;
		FSMState* nextState;
	};
}
