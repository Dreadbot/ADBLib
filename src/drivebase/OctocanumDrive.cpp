#include "OctocanumDrive.h"

namespace ADBLib
{
	/**
	 * @brief Constructor. Sets all solenoids and motors to nullptr.
	 */
	OctocanumDrive::OctocanumDrive() : Drivebase()
	{
		mode = mecanum;
		for (int i = 0; i < 4; ++i)
			solenoids[i] = nullptr;
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

	/**
	 * @brief Switches modes. Will
	 * @param newMode The mode to switch to.
	 */
	void OctocanumDrive::switchMode(driveMode newMode)
	{
		mode = newMode;
	}

	/**
	 * @brief Sets the pneumatic at a given position. Uses wheel-based positioning.
	 * @param newPneumatic The pneumatic to use.
	 * @param pos The position of the pneumatic.
	 */
	void OctocanumDrive::setPneumatic(SimplePneumatic* newPneumatic, MotorPos pos)
	{
		solenoids[pos] = newPneumatic;
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
