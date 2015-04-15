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
		void drive(float x = 0.0, float y = 0.0, float r = 0.0); //!< Drive with either traction or mecanum. Automatically extends or retracts pneumatics.
		void switchMode(driveMode newMode); //!< Allows switching from one mode to another.
		void setPneumatic(SimplePneumatic* newPneumatic, MotorPos pos); //!< Sets the pneumatic assigned to a single wheel. Based off of wheel positions.
		driveMode getMode(); //!< Returns the mode that the octocanum drive is currently in.
	protected:
		SimplePneumatic* solenoids[4];
		driveMode mode;
	};
}
