#pragma once

#include <WPILib.h>

namespace ADBLib
{
	class SimplePneumatic
	{
	public:


		SimplePneumatic();
		void set(DoubleSolenoid::Value value); //!< Smart set - automatically handles for invert
		void set(int dir); //!< Smart set - automatically handles for invert. -1 is kReverse, 1 is kForward, 0 is kOff, and anything non-zero is on for a single solenoid
		void setDouble(DoubleSolenoid* newDouble);
		void setSingle(Solenoid* newSingle);

	protected:
		enum PneumaticType {single, dual};

		DoubleSolenoid* dPneumatic;
		Solenoid* sPneumatic;
		PneumaticType type;
		bool invert;
	};
}
