#pragma once
#include <WPILib.h>

namespace ADBLib
{
	/**
	 * Stupid abstract base class for three-axis gyroscopes, which WPILib
	 * doesn't seem to support.
	 */
	class TriGyro
	{
	public:
		virtual ~TriGyro()=0;
		virtual double getXRate()=0;	//!< Returns in rad/s
		virtual double getYRate()=0;	//!< Returns in rad/s
		virtual double getZRate()=0;	//!< Returns in rad/s
		virtual void calibrate()=0;		//!< For any gyroscopes that need to be calibrated.
	};
}
