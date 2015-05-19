#include "Drivebase.h"

namespace ADBLib
{
	/**
	 * @brief Generic constructor; sets all motors to nullptr and sets the status as enabled.
	 */
	Drivebase::Drivebase()
	{
		enabled = true;
		for (int i = 0; i < 4; ++i)
		{
			motors[i] = nullptr;
			speeds[i] = 0.0;
		}
	}

	/**
	 * @brief Enables the drivebase.
	 */
	void Drivebase::enable()
	{
		enabled = true;
		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] != nullptr)
				motors[i]->enable();
		}
	}

	/**
	 * @brief Disables the drivebase and sets the command to zero the motor.
	 */
	void Drivebase::disable()
	{
		enabled = false;
		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] == nullptr)
				continue;
			motors[i]->disable();
			motors[i]->set(0);
		}
	}

	/**
	 * @brief Sets the motor at the designated position.
	 * @param motor A SimpleMotor object. Can be practically any kind of motor.
	 * @param position The position, as defined in MotorPos.
	 * @see MotorPos
	 */
	void Drivebase::setMotor(SimpleMotor* motor, MotorPos position)
	{
		motors[position] = motor;
	}

	/**
	 * @brief Sets all motors.
	 * @param mFrontLeft The front left motor.
	 * @param mFrontRight The front right motor.
	 * @param mBackRight The back right motor.
	 * @param mBackLeft The back left motor.
	 */
	void Drivebase::setMotors(SimpleMotor* mFrontLeft, SimpleMotor* mFrontRight, SimpleMotor* mBackRight, SimpleMotor* mBackLeft)
	{
		motors[frontLeft] = mFrontLeft;
		motors[frontRight] = mFrontRight;
		motors[backRight] = mBackRight;
		motors[backLeft] = mBackLeft;
	}

	/**
	 * @brief Gets the status of the drivebase.
	 * @return True if enabled, false if disabled.
	 */
	bool Drivebase::getEnabled()
	{
		return enabled;
	}

	/**
	 * @brief Internal function; normalizes wheel speeds to be between -1 and 1
	 */
	void Drivebase::normSpeeds()
	{
		float absSpeeds[4];
		for (int i = 0; i < 4; ++i)
			absSpeeds[i] = fabs(speeds[i]);

		float mag = *std::max_element(absSpeeds, absSpeeds + 4);
		if (mag > 1.0)
			for (int i = 0; i < 4; ++i)
				speeds[i] /= mag;
	}
}
