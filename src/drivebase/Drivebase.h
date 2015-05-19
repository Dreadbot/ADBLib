#pragma once
#include <WPILib.h>
#include <algorithm>
#include <string>
using std::string;

#include "../output/SimpleMotor.h"

/*
 * \brief Base abstract class for all drivebases to use.
 */
namespace ADBLib
{
	class Drivebase
	{
	public:
		enum MotorPos {frontLeft, frontRight, backRight, backLeft};
		const string MotorNames[4] = {"Front-left", "Front-Right", "Back-Right", "Back-Left"};

		Drivebase();
		virtual ~Drivebase() {};
		virtual void drive(float x = 0.0, float y = 0.0, float r = 0.0) = 0; //!< Pure virtual fuction; override this for drivebase-specific behavior
		virtual void enable();
		virtual void disable();
		virtual void setMotor(SimpleMotor* motor, MotorPos position);
		virtual void setMotors(SimpleMotor* mFrontLeft, SimpleMotor* mFrontRight, SimpleMotor* mBackRight, SimpleMotor* mBackLeft);
		bool getEnabled();
	protected:
		SimpleMotor* motors[4];
		bool enabled;
		float speeds[4];
		void normSpeeds();
	};
}
