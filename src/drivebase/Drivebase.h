#pragma once
#include <WPILib.h>

#include "../output/SimpleMotor.h"

/*
 * \brief Base abstract class for all drivebases to use.
 */
namespace ADBLib
{
	class Drivebase
	{
	public:
		enum motorPos {frontLeft, frontRight, backRight, backLeft};

		virtual Drivebase() = 0;
		virtual ~Drivebase() = 0;
		virtual void Drive(float x = 0.0, float y = 0.0, float r = 0.0) = 0;
	protected:
	};
}
