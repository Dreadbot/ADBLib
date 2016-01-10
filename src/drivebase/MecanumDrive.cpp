#include "MecanumDrive.h"

namespace ADBLib
{
	/**
	 * @brief Uses mecanum wheels to drive. Capable of full translation and rotation.
	 * @param x Strafing. Negative is left, positive is right. From -1 to 1.
	 * @param y Forward/Reverse. Positive is forward, negative is reverse. From 1 to -1.
	 * @param r Rotation. Positive is clockwise, negative is counterclockwise. From 1 to -1.
	 * @todo Test wheel rotations with a real mecanum drive
	 */
	void MecanumDrive::drive(float x, float y, float r)
	{

		speeds[frontLeft] = x + y + r;
		speeds[frontRight] = -x + y - r;
		speeds[backRight] = x + y - r;
		speeds[backLeft] = -x + y + r;

		speeds[frontLeft] = -speeds[frontLeft];
		speeds[backLeft] = -speeds[backLeft];

		normSpeeds();

		for (int i = 0; i < 4; ++i)
		{
			if (motors[i] != nullptr)
				motors[i]->Set(speeds[i] * 1000.f);
		}
	}
}
