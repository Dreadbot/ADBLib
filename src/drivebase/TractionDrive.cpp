#include "TractionDrive.h"

namespace ADBLib
{
	void TractionDrive::drive(float x, float y, float r)
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
	}
}
