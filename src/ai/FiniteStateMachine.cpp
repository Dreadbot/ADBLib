#include "FiniteStateMachine.h"

namespace ADBLib
{
	FiniteStateMachine::FiniteStateMachine()
	{
		currentState = nullptr;
	}

	/** Sets up the finite state machine with a new state table and a state to start from.
	 * @param newStateTable An array of FSMTransitions - the state table from which transition data will be drawn.
	 * @param initState The state from which the FSM should start.
	 * @note This process is not completely inexpensive (especially for large state tables). It converts an array to an std::multimap.
	 */
	void FiniteStateMachine::init(FSMTransition* newStateTable, FSMState* initState)
	{
		transitions.clear();
		for (auto state = &newStateTable[0]; state->currentState != nullptr; state++)
		{ //Since there is no direct conversion from an array of states to a multimap, this for loop does the job.
			pair<FSMState*, FSMTransition> transition(state->currentState, *state);
			transitions.insert(transition);
		}
		currentState = initState;
		initState->enter();
	}

	/**
	 * @brief Updates the current state and applies transitions when needed. Uses multimaps to enhance transition speed.
	 */
	void FiniteStateMachine::update()
	{
		int input = currentState->update();
		if (transitions.count(currentState) == 0)
			return; //Idiot check; this is a dead end state with no transitions away from it

		std::pair<multimap<FSMState*, FSMTransition>::iterator, multimap<FSMState*, FSMTransition>::iterator> range;
		range = transitions.equal_range(currentState); //Get all transitions with THIS key, which is the currentState.
		for (multimap<FSMState*, FSMTransition>::iterator iter = range.first; iter != range.second; iter++)
		{
			//Iterate through the list of elements under this key, which is a pointer to the current state
			if (iter->second.input == input)
			{
				//Regular non-blip transitioning.
				currentState->exit();
				currentState = iter->second.nextState;
				currentState->enter();
			}
		}
	}
}
