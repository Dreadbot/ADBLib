#pragma once

namespace ADBLib
{
	class FSMState
	{
	public:
		virtual ~FSMState() {};
		virtual void update() = 0;
		virtual void enter() = 0;
		virtual void exit() = 0;
	};
}
