#include "Drivebase.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; sets all motors.
	 * @param mFrontLeft The front left motor.
	 * @param mFrontRight The front right motor.
	 * @param mBackRight The back right motor.
	 * @param mBackLeft The back left motor.
	 * @note Accepts anything that inherits a SpeedController, so go wild.
	 */
	Drivebase::Drivebase(SpeedController* mFrontLeft,
			SpeedController* mFrontRight,
			SpeedController* mBackRight,
			SpeedController* mBackLeft)
	{
		enabled = true;
		motors[frontLeft] = mFrontLeft;
		motors[frontRight] = mFrontRight;
		motors[backRight] = mBackRight;
		motors[backLeft] = mBackLeft;

		for (int i = 0; i < 4; ++i)
			speeds[i] = 0.0;
	}

	/**
	 * @brief Deletes all motors.
	 */
	Drivebase::~Drivebase()
	{
		for (int i = 0; i < 4; i++)
			delete motors[i];
	}

	/**
	 * @brief Disables the drivebase and sets the command to zero the motor.
	 */
	void Drivebase::stop()
	{
		for (int i = 0; i < 4; ++i)
			motors[i]->Set(0);
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
		if (mag > 1.0) {
			for (int i = 0; i < 4; ++i)
				speeds[i] /= mag;
		}
	}
}
