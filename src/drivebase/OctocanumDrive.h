#pragma once

#include <WPILib.h>
#include "Drivebase.h"
#include "../output/SimplePneumatic.h"

namespace ADBLib
{
	/**
	 * @brief Octocanum drive class that allows for variable drive modes.
	 * Allows driving with both traction and mecanum wheels. This is accompished by using a form of rockers
	 * and pneumatic pistons to switch out wheels.
	 */
	class OctocanumDrive : public Drivebase
	{
		public:
			enum driveMode {mecanum, traction};

			OctocanumDrive(SpeedController* mFrontLeft,
					SpeedController* mFrontRight,
					SpeedController* mBackRight,
					SpeedController* mBackLeft,
					SimplePneumatic* pFrontLeft,
					SimplePneumatic* pFrontRight,
					SimplePneumatic* pBackRight,
					SimplePneumatic* pBackLeft);
			~OctocanumDrive();
			void drive(float x = 0.0, float y = 0.0, float r = 0.0);
			void switchMode(driveMode newMode);
			driveMode getMode();
		protected:
			SimplePneumatic* solenoids[4];
			driveMode mode;
	};
}
