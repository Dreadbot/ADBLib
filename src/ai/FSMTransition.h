#pragma once

#include "FSMState.h"

#define END_STATE_TABLE {nullptr, 0, false} //!< Append this to the end of your state tables; the init function will recognize it and stop searching the table for states.

namespace ADBLib
{
	class FSMTransition
	{
	public:
		FSMState* currentState;	//!< The current state.
		int input;				//!< If this input is recieved and the FSM state is the currentState, transition.
		FSMState* nextState;	//!< The next state to transition to.
	};
}
