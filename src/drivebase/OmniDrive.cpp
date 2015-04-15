#include "OmniDrive.h"

namespace ADBLib
{
	OmniDrive::OmniDrive()
	{
		transMotor = nullptr;
	}
	void OmniDrive::setTransMotor(SimpleMotor* newMotor)
	{
		transMotor = newMotor;
	}
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
