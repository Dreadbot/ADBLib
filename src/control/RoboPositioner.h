#pragma once

#include <WPILib.h>
#include "../../lib/hydra/Vector3D.h"
#include "../input/MPU6050.h"
#include "../../lib/MadgwickAHRS.h"

using Hydra::Vector3D;

#define gConv 9.80665

namespace ADBLib
{
	const double accFactors[4] = { //Convert to m/s^2
			0.0005985482f,	// +/- 2 g (16384 lsb/g)
			0.0011970964f,	// +/- 4 g (8192 lsb/g)
			0.0023941928f,	// +/- 8 g (4096 lsb/g)
			0.0047883855f	// +/- 16 g
	};
	const double gyrFactors[4] = { //Convert to rad/s
			1.3323124e-4f,
			2.6646248e-4f,
			5.3211258e-4f,
			0.0010642252f
	};
	const double magFactor = 0.3f; //Convert to uT

	/*
	 * \brief Robot positioning class that uses rotation and inertial measurements to determine location and velocity. All measurements in meters and related units.
	 */
	class RoboPositioner
	{
	public:
		enum rpsDir {X, Y, Z}; //External use
		RoboPositioner(MPU6050* newGyro); 	//!< Uses the built-in accelerometer and a given gyroscope.
		void resetInertFrame();				//!< Resets positions and rotations, but leaves velocity unchanged and adjusts velocity direction to compensate.
		double getPosition(rpsDir dir);		//!< Gets the current positions relative to the position of the most recent reset.
		Vector3D getVelocity();				//!< Gets the velocity relative to the ground.
		double getRotation(rpsDir dir);		//!< Gets the rotation relative to the starting rotation, in rads
		void update();						//!< Should be called regularly. Updates all calculations.
	protected:
		enum rpsData {gx = 0, gy, gz, ax, ay, az, mx, my, mz};

		double positions[3];	//Positions in meters
		double rotations[3];	//Rotations in rads
		double startRots[3];	//Starting rotations
		Vector3D vel;

		Timer interval;
		MPU6050* gyro;
		MadgwickAHRS mdgw;
	};
}
