#include "Path.h"

namespace ADBLib
{
	/**
	 * @brief Constructor. Accepts any drivebase and any RoboPositioner.
	 * @param newDrivebase Any drivebase. Settings for using it specified on a waypoint basis.
	 * @param newRPS The RoboPositioner. You'll need to update this outside the Path class.
	 * @note While the Path is running, do NOT reset the RPS. If you do, you need to reset the path as well.
	 */
	Path::Path(Drivebase* newDrivebase, RoboPositioner* newRPS)
	{
		drivebase = newDrivebase;
		rps = newRPS;
		active = false;
		tolerance = 0.25; //meters
		reset();
	}

	/**
	 * @brief Reset the path to start at the first waypoint and reset the positions.
	 */
	void Path::reset()
	{
		currentWP = waypoints.begin();
		for (int i = 0; i < 3; i++)
			startPos[(rpsDir)i] = rps->getPosition((rpsDir)i);
	}

	/** Adds a new waypoint to the end of the waypoint list. First in, first out.
	 * @param xLoc The X location, in meters.
	 * @param yLoc The Y location, in meters.
	 * @param tSpeed The translational speed of the robot, from 0 to 1. As passed to the drivebase.
	 * @param rSpeed The rotational speed of the robot, from 0 to 1. As passed to the drivebase.
	 * @param smode The strafing mode. Don't use strafing if the drivebase doesn't support strafing.
	 * @param rmode The relativity mode. WP_START_RELATIVE is relative to the start of the path, WPMODE_WP_RELATIVE is relative to the last waypoint.
	 */
	void Path::pushWaypoint(double xLoc, double yLoc, double tSpeed, double rSpeed, waypoint::srMode smode, waypoint::rlMode rmode)
	{
		waypoint nwp {xLoc, yLoc, tSpeed, rSpeed, smode, rmode};
		waypoints.push_back(nwp);
	}

	void Path::pushWaypoint(waypoint nwp)
	{
		waypoints.push_back(nwp);
	}

	void Path::setTolerance(double meters)
	{
		tolerance = meters;
	}

	/** Updates the RPS and uses data from it to compute the proper path to go to the waypoints in series.
	 * @return True if the path is completed, false if the path is incomplete.
	 * @todo Graphically simulate
	 */
	bool Path::executePath()
	{
		if (currentWP == waypoints.end())
			return true;

		waypoint wp = *currentWP;
		Vector3D dir(0, 0, 0); //Vector3D uses 3D, but waypoints just use a regular ol' cartesian coordinate plane (no quadcopters yet, sorry!)
		if (wp.rmode == waypoint::RLMODE_WP_RELATIVE && currentWP != waypoints.begin())
		{ //Relative to last waypoint
			vector<waypoint>::iterator temp = currentWP--; //TODO: Verify that this is a thing. It should be.
			dir.setX(temp->xLoc - rps->getPosition(rpsDir::X));
			dir.setZ(temp->yLoc - rps->getPosition(rpsDir::Z));
		}
		else
		{ //Relative to start position
			dir.setX(startPos[rpsDir::X] - rps->getPosition(rpsDir::X));
			dir.setZ(startPos[rpsDir::Z] - rps->getPosition(rpsDir::Z));
		}

		if (dir.mag() <= tolerance)
		{
			currentWP++;
			return false; //Distance within tolerance; advance waypoints.
		}

		double rotDiff = rps->getRotation(rpsDir::Y) - dir.getAngleZ(); //Iffy -- possible problem with the bit on getRotation.
		Vector3D robotDir(0, 0, wp.tSpeed);
		robotDir.rotateY(-rotDiff); //Take the robot's needed velocity, set it to 90 degrees, and rotate it by the rotation difference. This way, translation will work normally.

		//Actual drive commands
		if (wp.smode == waypoint::SRMODE_STRAFE_ONLY)
			drivebase->drive(robotDir.getX(), robotDir.getZ(), 0.f); //Again, Z becomes Y when converting from 3D to 2D.
		else if (wp.smode == waypoint::SRMODE_ROTATE_ONLY)
			drivebase->drive(0, wp.tSpeed, rotDiff > 0 ? -wp.rSpeed : wp.rSpeed); //TODO: Test to see if the robot jiggles too much
		else //waypoint::SRMODE_STRAFE_ROTATE
			drivebase->drive(robotDir.getX(), robotDir.getZ(), rotDiff > 0 ? -wp.rSpeed : wp.rSpeed);

		return false;
	}
}
