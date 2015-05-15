#include "RoboPositioner.h"

namespace ADBLib
{
	RoboPositioner::RoboPositioner(MPU6050* newGyro)
	{
		gyro = newGyro;
		gyro->initialize();

		//Assumes robot velocities are at zero.
		vel.setX(0);
		vel.setY(0);
		vel.setZ(0);

		for (int i = 0; i < 3; i++)
		{
			positions[i] = 0;
			startRots[i] = 0;
		}
	}
	void RoboPositioner::resetInertFrame()
	{
		//Reset positions to 0
		for (int i = 0; i < 3; i++)
			positions[i] = 0.0;

		/* Rotation is more complicated. Velocities are stored
		 * relative to the starting rotation. Velocities must
		 * be rotated around to compensate for this.*/

		double rotDiffs[3];
		int16_t temp[3];
		gyro->getRotation(&temp[gx], &temp[gy], &temp[gz]); //Works because gx, gy, and gz are 0, 1, and 2, respectively.
		for (int i = 0; i < 3; i++)
			rotDiffs[3] = q15toFloat(temp[i]) - startRots[i];

		//Rotate velocity vector to compensate for new starting rotations
		//TODO: Determine if this correction works.
		vel.rotateX(-rotDiffs[gx]);
		vel.rotateY(-rotDiffs[gy]);
		vel.rotateZ(-rotDiffs[gz]);

		//Reset rotations
		for (int i = 0; i < 3; i++)
			startRots[i] = q15toFloat(temp[i]);
	}
	double RoboPositioner::getPosition(rpsDir dir)
	{
		return positions[dir];
	}
	Vector3D RoboPositioner::getVelocity()
	{
		return vel;
	}
	double RoboPositioner::getRotation(rpsDir dir)
	{
		//Since we don't know WHICH rotation to grab, grab all of 'em!
		return rotations[dir] - startRots[dir];
	}
	void RoboPositioner::update()
	{
		//Grab values from MPU
		float vals[9];
		int16_t temp[9];

		interval.Stop(); //Time interval up.
		double time = interval.Get(); //Double time... couldn't resist it.
		interval.Reset();
		interval.Start();

		gyro->getMotion9(
				&temp[ax], &temp[ay], &temp[az], 	//Accelerometer
				&temp[gx], &temp[gy], &temp[gz], 	//Gyroscope
				&temp[mx], &temp[my], &temp[mz]);	//Magnetometer (WTH???)

		//Convert to floating point numbers
		for (int i = 0; i < 3; i++)
			vals[i] = (float)temp[i] * accFactors[1]; //Max 4g
		for (int i = 3; i < 6; i++)
			vals[i] = (float)temp[i] * gyrFactors[0]; //Max 250 dps (degrees per second)
		for (int i = 6; i < 9; i++)
			vals[i] = (float)temp[i] * magFactor;


		//Run a Madgwick filter to dramatically increase accuracy of readings
		mdgw.update(
				vals[gx], vals[gy], vals[gz],	//Gyroscope
				vals[ax], vals[ay], vals[az],	//Accelerometer
				vals[mx], vals[my], vals[mz]);	//Magnetometer (no really, WTH?)

		//The values in the array must now be updated to the filter's rotation values...which are stored as a quaternion.
		//http://www.tinkerforge.com/en/doc/Software/Bricks/IMU_Brick_CSharp.html
		vals[gx] = atan2(2*mdgw.q2*mdgw.q0 - 2*mdgw.q1*mdgw.q3, 1 - 2*mdgw.q2*mdgw.q2 - 2*mdgw.q3*mdgw.q3);
		vals[gy] = atan2(2*mdgw.q1*mdgw.q0 - 2*mdgw.q2*mdgw.q3, 1 - 2*mdgw.q1*mdgw.q1 - 2*mdgw.q3*mdgw.q3);
		vals[gz] = asin(2*mdgw.q1*mdgw.q2 + 2*mdgw.q3*mdgw.q0);

		//Add the current velocity's values to the previous position
		positions[X] += vel.getX() * time;
		positions[Y] += vel.getY() * time;
		positions[Z] += vel.getZ() * time;
		for (int i = 0; i < 3; i++)
			rotations[3] += vals[i + 3] * time; //Hopefully add current rotation?

		//Assemble a vector with acceleration in gees converted to m/s (?), then correct for rotation.
		Vector3D newVel(vals[ax] * gConv * time, vals[ay] * gConv * time, vals[az] * gConv * time); //TODO: Test to make sure this works
		newVel.rotateX(-vals[gx]);
		newVel.rotateY(-vals[gx]);
		newVel.rotateZ(-vals[gz]);

		vel = vel + newVel; //Finalize current velocity.
	}
}
