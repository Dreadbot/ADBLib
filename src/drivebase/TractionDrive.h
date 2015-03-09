#pragma once
#include <WPILib.h>
#include "Drivebase.h"

namespace ADBLib
{
	/*
	 * \brief Simple TractionDrive class for driving with a traction drive. Discards x velocity.
	 */
	class TractionDrive : public Drivebase
	{
	public:
		void drive(float x = 0.0, float y = 0.0, float r = 0.0);
	};
}
