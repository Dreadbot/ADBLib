#include "OmniDrive.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; sets all motors, including the translation motor.
	 * @param mFrontLeft The front left motor.
	 * @param mFrontRight The front right motor.
	 * @param mBackRight The back right motor.
	 * @param mBackLeft The back left motor.
	 * @param newTransMotor The translation motor.
	 */
	OmniDrive::OmniDrive(SpeedController* mFrontLeft,
			SpeedController* mFrontRight,
			SpeedController* mBackRight,
			SpeedController* mBackLeft,
			SpeedController* newTransMotor) : Drivebase(mFrontLeft,mFrontRight, mBackRight, mBackLeft)
	{
		transMotor = newTransMotor;
	}

	/**
	 * @brief Deletes all motors, including the translation motor.
	 */
	OmniDrive::~OmniDrive()
	{
		for (int i = 0; i < 4; i++)
			delete motors[i];
		delete transMotor;
	}

	/**
	 * @brief Set the translation motor.
	 * @param newMotor The translational motor. Should be set perendicular to the set of regular omni wheels.
	 */
	void OmniDrive::setTransMotor(SpeedController* newMotor)
	{
		transMotor = newMotor;
	}

	/**
	 * @brief Uses omni wheels to drive in combination with a translational omni wheel. Capable of full translation and rotation.
	 * @param x Strafing. Negative is left, positive is right. From -1 to 1.
	 * @param y Forward/Reverse. Positive is forward, negative is reverse. From 1 to -1.
	 * @param r Rotation. Positive is clockwise, negative is counterclockwise. From 1 to -1.
	 */
	void OmniDrive::drive(float x, float y, float r)
	{
		speeds[frontLeft] = y + r;
		speeds[frontRight] = y - r;
		speeds[backRight] = -(y - r);
		speeds[backLeft] = -(y + r);
		normSpeeds();

		for (int i = 0; i < 4; ++i) {
			if (motors[i] != nullptr)
				motors[i]->Set(speeds[i]);
		}

		if (transMotor != nullptr)
			transMotor->Set(x);
	}

	void OmniDrive::stop()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] == nullptr)
				continue;
			motors[i]->Set(0);
		}
	}
}
