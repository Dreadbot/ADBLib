#include "TractionDrive.h"

namespace ADBLib
{
	/**
	 * @brief Uses traction wheels to drive. Capable of forward/reverse and left/right.
	 * @param x Discarded; traction drive incapable of strafing.
	 * @param y Forward/Reverse. Positive is forward, negative is reverse. From 1 to -1.
	 * @param r Rotation. Positive is clockwise, negative is counterclockwise. From 1 to -1.
	 */
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
