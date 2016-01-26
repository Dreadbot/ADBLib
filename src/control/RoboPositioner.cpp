#include "RoboPositioner.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; accepts any gyroscope and accelerometer
	 * @param newGyro A TriGyro, subclass the TriGyro class to account for your specific gyroscope setup
	 * @param newAcclro An Accelerometer object, subclass the Accelerometer class to account for your specific accelerometer setup.
	 */
	RoboPositioner::RoboPositioner(TriGyro* newGyro, Accelerometer* newAcllro)
	{
		gyro = newGyro;
		acllro = newAcllro;

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

	/**
	 * @brief Resets positions and rotations, and adjusts velocity to compensate.
	 */
	void RoboPositioner::resetInertFrame()
	{
		//Reset positions to 0
		for (int i = 0; i < 3; i++)
			positions[i] = 0.0;

		/* Rotation is more complicated. Velocities are stored
		 * relative to the starting rotation, and must
		 * be rotated around to compensate for this.*/

		//Rotate velocity vector to compensate for new starting rotations
		//TODO: Determine if this correction works.
		vel.rotateX(-(rotations[X] - startRots[X]));
		vel.rotateY(-(rotations[Y] - startRots[Y]));
		vel.rotateZ(-(rotations[Z] - startRots[Z]));

		//Reset rotations
		for (int i = 0; i < 3; i++)
			startRots[i] = 0;
	}

	/**
	 * @brief Gets the position relative to the position of the most resent reset.
	 * @param dir The direction, X, Y, or Z.
	 * @return Distance from, in meters.
	 */
	double RoboPositioner::getPosition(rpsDir dir)
	{
		return positions[dir];
	}

	/**
	 * @brief Gets the velocity vector of the robot.
	 * @return A Vector3D object for the velocity.
	 */
	Vector3D RoboPositioner::getVelocity()
	{
		return vel;
	}

	/**
	 * @brief Gets the rotation of the robot.
	 * @param dir The direction to get rotation from.
	 * @return Rotation relative to the starting rotation, in radians.
	 * @note To get yaw (left/right), get rotation along the Z axis (?).
	 */
	double RoboPositioner::getRotation(rpsDir dir)
	{
		return rotations[dir];
	}

	/**
	 * @brief Updates the robot position, rotation, and velocity.
	 * @note You need to call this function iteratively; the shorter the interval,
	 * the more accurate it will be. It handles timing on its own. It is reccomended
	 * to set up a notifier loop thingy for this.
	 */
	void RoboPositioner::update()
	{
		//Grab values from MPU
		float vals[6];

		interval.Stop(); //Time interval up.
		double time = interval.Get(); //Double time... couldn't resist it.
		interval.Reset();
		interval.Start();

		vals[ax] = acllro->GetX();
		vals[ay] = acllro->GetY();
		vals[az] = acllro->GetZ();
		vals[gx] = gyro->getXRate();
		vals[gy] = gyro->getYRate();
		vals[gz] = gyro->getZRate();

		//Run a Madgwick filter to dramatically increase accuracy of readings
		mdgw.updateIMU(vals[gx], vals[gy], vals[gz],
					   vals[ax], vals[ay], vals[az]);

		//The values in the array must now be updated to the filter's rotation values...which are stored as a quaternion.
		//http://www.tinkerforge.com/en/doc/Software/Bricks/IMU_Brick_CSharp.html
		mdgw.getEuler(vals[gx], vals[gy], vals[gz]);

		//Add the current velocity's values to the previous position
		positions[X] += vel.getX() * time;
		positions[Y] += vel.getY() * time;
		positions[Z] += vel.getZ() * time;
		for (int i = 0; i < 3; i++)
			rotations[3] += vals[i + 3] * time; //Hopefully add current rotation?

		//Assemble a vector with acceleration in gees converted to m/s (?), then correct for rotation.
		Vector3D newVel(vals[ax] * gConv * time, vals[ay] * gConv * time, vals[az] * gConv * time); //TODO: Test to make sure this works
		newVel.rotateX(-rotations[X]);
		newVel.rotateY(-rotations[Y]);
		newVel.rotateZ(-rotations[Z]);

		vel = vel + newVel; //Finalize current velocity.
	}
}
