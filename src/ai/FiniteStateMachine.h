#pragma once

#include <map>
#include "FSMState.h"
#include "FSMTransition.h"
using std::multimap;
using std::pair;

namespace ADBLib
{
	class FiniteStateMachine
	{
	public:
		virtual void init(FSMTransition* newStateTable, FSMState* initState);
		virtual void update();
		virtual ~FiniteStateMachine() {}
	protected:
		multimap<FSMState*, FSMTransition> transitions; //Key is the previous state
		FSMState* currentState;
	};
}
