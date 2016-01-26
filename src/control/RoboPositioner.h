#pragma once

#include <WPILib.h>
#include "../misc/Vector3D.h"
#include "../input/TriGyro.h"
#include "../../lib/MadgwickAHRS.h"

namespace ADBLib
{
	/**
	 * Robot positioning class that uses rotation and inertial measurements to determine
	 * location and velocity. All measurements in SI units.
	 */
	class RoboPositioner
	{
	public:
		enum rpsDir {X, Y, Z}; 			//!< For usage by stuff outside of RoboPositioner.
		const double gConv = 9.80665;

		RoboPositioner(TriGyro* newGyro, Accelerometer* newAcllro);
		void resetInertFrame();
		double getPosition(rpsDir dir);
		Vector3D getVelocity();
		double getRotation(rpsDir dir);
		void update();
	protected:
		enum rpsData {gx = 0, gy, gz, ax, ay, az, mx, my, mz};

		double positions[3];	//Positions in meters
		double rotations[3];	//Rotations in rads
		double startRots[3];	//Starting rotations
		Vector3D vel;

		TriGyro* gyro;
		Accelerometer* acllro;
		Timer interval;

		MadgwickAHRS mdgw;
	};
}
