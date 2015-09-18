#include "OctocanumDrive.h"

namespace ADBLib
{
	OctocanumDrive::OctocanumDrive(SpeedController* mFrontLeft,
			SpeedController* mFrontRight,
			SpeedController* mBackRight,
			SpeedController* mBackLeft,
			SimplePneumatic* pFrontLeft,
			SimplePneumatic* pFrontRight,
			SimplePneumatic* pBackRight,
			SimplePneumatic* pBackLeft) : Drivebase(
					mFrontLeft, mFrontRight,mBackRight,mBackLeft)
	{
		mode = mecanum;
		solenoids[frontLeft]	= pFrontLeft;
		solenoids[frontRight] 	= pFrontRight;
		solenoids[backLeft]		= pBackLeft;
		solenoids[backRight]	= pBackRight;
	}

	OctocanumDrive::~OctocanumDrive()
	{
		for (int i = 0; i < 4; i++)
		{
			delete solenoids[i];
			delete motors[i];
		}
	}

	/**
	 * @brief Drives with full translational and rotational capability.
	 * @param x Strafing. Negative is left, positive is right. From -1 to 1. Only available in mecanum mode.
	 * @param y Forward/Reverse. Positive is forward, negative is reverse. From 1 to -1.
	 * @param r Rotation. Positive is clockwise, negative is counterclockwise. From 1 to -1.
	 */
	void OctocanumDrive::drive(float x, float y, float r)
	{
		if (mode == mecanum)
		{
			speeds[frontLeft] = -(x + y + r);
			speeds[frontRight] = x + y - r;
			speeds[backRight] = x + y - r;
			speeds[backLeft] = -(x + y + r);

			normSpeeds();

			for (int i = 0; i < 4; i++)
			{
				if (motors[i] != nullptr)
					motors[i]->Set(speeds[i]);
				if (solenoids[i] != nullptr)
					solenoids[i]->set(0); //Pull traction wheels UP
			}

		}
		if (mode == traction)
		{

			speeds[frontLeft] = y + r;
			speeds[frontRight] = y - r;
			speeds[backRight] = -(y - r);
			speeds[backLeft] = -(y + r);

			normSpeeds();

			for (int i = 0; i < 4; ++i)
			{
				if (motors[i] != nullptr)
					motors[i]->Set(speeds[i]);
				if (solenoids[i] != nullptr)
					solenoids[i]->set(1); //Push traction wheels DOWN
			}
		}
	}

	/**
	 * @brief Switches modes. Will
	 * @param newMode The mode to switch to.
	 */
	void OctocanumDrive::switchMode(driveMode newMode)
	{
		mode = newMode;
	}

	/**
	 * @brief Gets the current mode of the drivebase.
	 * @return The mode.
	 */
	OctocanumDrive::driveMode OctocanumDrive::getMode()
	{
		return mode;
	}
}
