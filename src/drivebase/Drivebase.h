#pragma once
#include <WPILib.h>

#include "../output/SimpleMotor.h"
#include "../../lib/hydra/hmath.h"

/*
 * \brief Base abstract class for all drivebases to use.
 */
namespace ADBLib
{
	class Drivebase
	{
	public:
		enum MotorPos {frontLeft, frontRight, backRight, backLeft};

		Drivebase();
		virtual ~Drivebase() = 0; //!< Override this constructor if necessary. Do NOT delete anything in the motors array!
		virtual void drive(float x = 0.0, float y = 0.0, float r = 0.0) = 0; //!< Override this for drivebase-specific behavior.
		virtual void enable();	//!< Enables all drive motors, regardless of whether they are CAN or PWM.
		virtual void disable(); //!< Disables all drive motors, regardless of whether they are CAN or PWM.
		virtual void setMotor(SimpleMotor* motor, MotorPos position); //!< Allows the setting of an individual motor.
		virtual void setMotors(SimpleMotor* mFrontLeft, SimpleMotor* mFrontRight, SimpleMotor* mBackRight, SimpleMotor* mBackLeft); //!< Allows setting of all four motors at once.
		void normSpeeds();

		bool getEnabled(); //!< Gets the status of the drivebase - enabled or disabled.
	protected:
		SimpleMotor* motors[4];
		bool enabled;
		float speeds[4];
	};
}
