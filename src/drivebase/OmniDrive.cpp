#include "OmniDrive.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; sets the translational motor to nullptr.
	 */
	OmniDrive::OmniDrive() : Drivebase()
	{
		transMotor = nullptr;
	}

	/**
	 * @brief Set the translation motor.
	 * @param newMotor The translational motor. Should be set perendicular to the set of regular omni wheels.
	 */
	void OmniDrive::setTransMotor(SimpleMotor* newMotor)
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
		motors[backLeft]->setInvert(true); //Experimental
		motors[backRight]->setInvert(true);

		speeds[frontLeft] = y + r;
		speeds[frontRight] = y - r;
		speeds[backRight] = y - r;
		speeds[backLeft] = y + r;

		normSpeeds();

		for (int i = 0; i < 4; ++i)
			if (motors[i] != nullptr)
				motors[i]->set(speeds[i]);

		if (transMotor != nullptr)
			transMotor->set(x);
	}
}
