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
		virtual ~Drivebase() = 0;
		virtual void drive(float x = 0.0, float y = 0.0, float r = 0.0) = 0;
		virtual void enable();
		virtual void disable();
		virtual void setMotor(SimpleMotor* motor, MotorPos position);
		virtual void setMotors(SimpleMotor* mFrontLeft, SimpleMotor* mFrontRight, SimpleMotor* mBackRight, SimpleMotor* mBackLeft);

		bool getEnabled();
	protected:
		SimpleMotor* motors[4];
		bool enabled;
	};
}
