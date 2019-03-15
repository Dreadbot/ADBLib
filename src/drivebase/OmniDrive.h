#pragma once
#include <WPILib.h>
#include "Drivebase.h"

namespace ADBLib
{
	/**
	 * @brief Simple omni drive class that uses omni wheels and a translational omni wheel for strafing.
	 */
	class OmniDrive : public Drivebase
	{
		public:
			OmniDrive(SpeedController* mFrontLeft,
					SpeedController* mFrontRight,
					SpeedController* mBackRight,
					SpeedController* mBackLeft,
					SpeedController* newTransMotor);
			~OmniDrive();
			void drive(float x = 0.0, float y = 0.0, float r = 0.0);
			void setTransMotor(SpeedController* newMotor);
			void stop();
		protected:
			SpeedController* transMotor;
	};
}
