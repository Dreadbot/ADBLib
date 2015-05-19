#pragma once
#include <WPILib.h>
#include "Drivebase.h"

namespace ADBLib
{
	/**
	 * @brief Mecanum drive class that uses mecanum wheels for full translation and rotation.
	 */
	class MecanumDrive : public Drivebase
	{
	public:
		void drive(float x = 0.0, float y = 0.0, float r = 0.0);
	};
}
