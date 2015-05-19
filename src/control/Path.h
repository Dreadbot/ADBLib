#pragma once

#include <WPILib.h>
#include <vector>
#include "RoboPositioner.h"
#include "../drivebase/Drivebase.h"
using std::vector;

namespace ADBLib
{
	struct waypoint
	{
		enum rlMode {RLMODE_START_RELATIVE, RLMODE_WP_RELATIVE};
		enum srMode {SRMODE_STRAFE_ONLY, SRMODE_STRAFE_ROTATE, SRMODE_ROTATE_ONLY};

		double xLoc; 		//!< The X position to transition to. Computed in meters.
		double yLoc; 		//!< The Y position to transition to. Computer in meters.
		double tSpeed;		//!< The translational speed of the robot, from 0 to 1. As passed to the drivebase.
		double rSpeed;		//!< The rotational speed of the robot, from 0 to 1. As passed to the drivebase.
		srMode smode;		//!< The strafing mode. Don't use strafing if the drivebase doesn't support strafing.
		rlMode rmode;		//!< The relativity mode. WP_START_RELATIVE is relative to the start of the path, WPMODE_WP_RELATIVE is relative to the last waypoint.
	};

	/*
	 * An object for storing a series of waypoints that the robot can automatically move to in sequence.
	 */
	class Path
	{
	public:
		Path(Drivebase* newDrivebase, RoboPositioner* newRPS);
		void reset(); //!< Resets the starting location to the current position, and moves the waypoint iterator to the beginning
		void pushWaypoint(double xLoc, double yLoc, double tSpeed, double rSpeed, waypoint::srMode smode, waypoint::rlMode rmode);
		void pushWaypoint(waypoint nwp);
		void setTolerance(double meters); //!< Sets the distance tolerance.
		bool executePath();
	protected:
		vector<waypoint> waypoints;
		vector<waypoint>::iterator currentWP;
		Drivebase* drivebase;
		RoboPositioner* rps;
		double startPos[3];
		double tolerance;
		bool active;
	};
}
