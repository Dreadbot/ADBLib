#pragma once

#include <WPILib.h>
#include "SimpleMotor.h"

namespace ADBLib
{
	class PIDMotor : public SimpleMotor
	{
	public:
		enum PIDK {Kp, Ki, Kd};

		PIDMotor(SpeedController* newMotor, PIDSource* newSrc);
		PIDMotor(double P, double I, double D, PIDOutput* newMotor, PIDSource* newSrc);
		~PIDMotor();

		//SimpleMotor overrides
		void set(float value);

		//Useful PID things from the PIDController class
		float getError();
		bool isOnTarget();
		void setAbsToler(float tolerance); //!< Sets the absolute value tolerance.
		void setContinuous(bool newCont); //!< Sets whether this PID motor is continuous or no. See documentation for PIDController in WPILib
		void setInputRange(float newMax, float newMin); //!< Sets the input range expected from the sensor.
		void setOutputRange(float newMax, float newMin); //!< Sets the output range that the output expects
		void setPercentTolerance(float newPToler); //!< Sets the percent tolerance of the PID controller.

		//PIDMotor-specific stuff
		void setPID(double newP, double newI, double newD); //!< Sets ALL the PID constants!
		void setPeriod(float newPeriod); //!< Sets the update period (?)
		void setSource(PIDSource* newSource); //!< Sets the PID input source. This should be a sensor.
	protected:
		void setupCtrl(); //!< Internal function; creates a new PID since the only way to set a bunch of stuff is through the constructor for PIDController

		double PIDValues[3];
		float period;
		PIDController* pidctrl;
		PIDSource* source;
		PIDOutput* motor;
	};
}
