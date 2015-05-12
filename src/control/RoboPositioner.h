#pragma once

#include <WPILib.h>
#include "../../lib/hydra/Vector3D.h"
#include "../input/MPU6050.h"
#include "../../lib/MadgwickAHRS.h"

using Hydra::Vector3D;

#define gConv 9.80665

namespace ADBLib
{
	/*
	 * \brief Robot positioning class that uses rotation and inertial measurements to determine location and velocity. All measurements in meters and related units.
	 *
	 */
	class RoboPositioner
	{
	public:
		enum rpsDir {X, Y, Z}; //External use
		RoboPositioner(MPU6050* newGyro); 	//!< Uses the built-in accelerometer and a given gyroscope.
		void resetInertFrame();				//!< Resets positions and rotations, but leaves velocity unchanged and adjusts velocity direction to compensate.
		double getPosition(rpsDir dir);	//!< Gets the current positions relative to the position of the most recent reset.
		double getVelocity(rpsDir dir);	//!< Gets the velocity relative to the ground.
		double getRotation(rpsDir dir);	//!< Gets the rotation relative to the starting rotation.
		void update();						//!< Should be called regularly. Updates all calculations.
	protected:
		enum rpsData {gx = 0, gy, gz, ax, ay, az, mx, my, mz};

		double positions[3];
		double startRots[3];	//Starting rotations
		Vector3D vel;

		Timer interval;
		MPU6050* gyro;
	};
}
