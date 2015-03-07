#include "MecanumDrive.h"

namespace ADBLib
{
	void MecanumDrive::drive(float x, float y, float r)
	{
		motors[frontRight]->setInvert(true);
		motors[backLeft]->setInvert(true);

		speeds[frontLeft] = x + y + r;
		speeds[frontRight] = x + y - r;
		speeds[backRight] = x + y - r;
		speeds[backLeft] = x + y + r;

		normSpeeds();

		for (int i = 0; i < 4; i++)
			motors[i]->set(speeds[i]);
	}
}
