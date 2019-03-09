#pragma once
#include <WPILib.h>
#include <algorithm>
#include <string>
using std::string;

/*
 * \brief Base abstract class for all drivebases to use.
 */
namespace ADBLib
{
	class Drivebase
	{
		public:
			enum MotorPos {frontLeft, frontRight, backRight, backLeft};
			const string MotorNames[4] = {"Front-left", "Front-Right", "Back-Right", "Back-Left"}; //!< Motor names that correspond to the MotorPos array; can be used for debug purposes.

			Drivebase(SpeedController* mFrontLeft,
					SpeedController* mFrontRight,
					SpeedController* mBackRight,
					SpeedController* mBackLeft);
			virtual ~Drivebase();
			virtual void drive(float x = 0.0, float y = 0.0, float r = 0.0) = 0; //!< Pure virtual function; override this for drivebase-specific behavior
			virtual void stop();
			bool getEnabled();
		protected:
			SpeedController* motors[4];
			bool enabled;
			float speeds[4];
			void normSpeeds();
	};
}
