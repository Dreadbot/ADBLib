#pragma once

#include <WPILib.h>

namespace ADBLib
{
	class SimplePneumatic
	{
		public:
			SimplePneumatic();
			SimplePneumatic(DoubleSolenoid* newDouble);
			SimplePneumatic(Solenoid* newSingle);
			void set(DoubleSolenoid::Value value);
			void set(int dir);
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
