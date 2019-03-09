#pragma once

namespace ADBLib
{
	class FSMState
	{
		public:
			virtual ~FSMState() {};
			virtual int update() = 0;	//!< The update function. Override this to add behavior to your states.
			virtual void enter() = 0;	//!< Called when this state is entered.
			virtual void exit() = 0;	//!< Called when this state is exited.
	};
}
