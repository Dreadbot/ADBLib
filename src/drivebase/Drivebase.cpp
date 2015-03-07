#include "Drivebase.h"

namespace ADBLib
{
	Drivebase::Drivebase()
	{
		enabled = true;
		for (int i = 0; i < 4; ++i)
		{
			motors[i] = nullptr;
		}
	}
	void Drivebase::enable()
	{
		enabled = true;
		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] != nullptr)
				motors[i]->enable();
		}
	}
	void Drivebase::disable()
	{
		enabled = false;
		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] != nullptr)
				motors[i]->disable();
		}
	}
	void Drivebase::setMotor(SimpleMotor* motor, MotorPos position)
	{
		motors[position] = motor;
	}
	void Drivebase::setMotors(SimpleMotor* mFrontLeft, SimpleMotor* mFrontRight, SimpleMotor* mBackRight, SimpleMotor* mBackLeft)
	{
		motors[frontLeft] = mFrontLeft;
		motors[frontRight] = mFrontRight;
		motors[backRight] = mBackRight;
		motors[backLeft] = mBackLeft;
	}
	bool Drivebase::getEnabled()
	{
		return enabled;
	}
	void Drivebase::normSpeeds()
	{
		float absSpeeds[4];
		for (int i = 0; i < 4; ++i)
			absSpeeds = fabs(speeds[i]);

		float mag = *std::max_element(absSpeeds, absSpeeds + 4);
		if (mag > 1.0)
			for (int i = 0; i < 4; ++i)
				speeds[i] /= mag;
	}
}
