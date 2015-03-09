#include "OctocanumDrive.h"

namespace ADBLib
{
	OctocanumDrive::OctocanumDrive()
	{
		mode = mecanum;
		for (int i = 0; i < 4; ++i)
			solenoids[i] = nullptr;
	}
	void OctocanumDrive::drive(float x, float y, float r)
	{
		if (mode == mecanum)
		{
			motors[frontRight]->setInvert(true);
			motors[backLeft]->setInvert(true);
			motors[backRight]->setInvert(false);
			motors[frontLeft]->setInvert(false);

			speeds[frontLeft] = x + y + r;
			speeds[frontRight] = x + y - r;
			speeds[backRight] = x + y - r;
			speeds[backLeft] = x + y + r;

			normSpeeds();

			for (int i = 0; i < 4; i++)
			{
				if (motors[i] != nullptr)
					motors[i]->set(speeds[i]);
				if (solenoids[i] != nullptr)
					solenoids[i]->set(0); //Pull traction wheels UP
			}

		}
		if (mode == traction)
		{
			motors[backLeft]->setInvert(true); //Experimental
			motors[backRight]->setInvert(true);
			motors[frontLeft]->setInvert(false);
			motors[frontRight]->setInvert(false);

			speeds[frontLeft] = y + r;
			speeds[frontRight] = y - r;
			speeds[backRight] = y - r;
			speeds[backLeft] = y + r;

			normSpeeds();

			for (int i = 0; i < 4; ++i)
			{
				if (motors[i] != nullptr)
					motors[i]->set(speeds[i]);
				if (solenoids[i] != nullptr)
					solenoids[i]->set(1); //Push traction wheels DOWN
			}
		}
	}
	void OctocanumDrive::switchMode(driveMode newMode)
	{
		mode = newMode;
	}
	void OctocanumDrive::setPneumatic(SimplePneumatic* newPneumatic, MotorPos pos)
	{
		solenoids[pos] = newPneumatic;
	}
	OctocanumDrive::driveMode OctocanumDrive::getMode()
	{
		return mode;
	}
}
