#pragma once

#include <WPILib.h>
#include "Drivebase.h"
#include "../output/SimplePneumatic.h"

namespace ADBLib
{
	class OctocanumDrive : public Drivebase
	{
	public:
		enum driveMode {mecanum, traction};

		OctocanumDrive();
		void drive(float x = 0.0, float y = 0.0, float r = 0.0);
		void switchMode(driveMode newMode);
		void setPneumatic(SimplePneumatic* newPneumatic, MotorPos pos);
		driveMode getMode();
	protected:
		SimplePneumatic* solenoids[4];
		driveMode mode;
	};
}
