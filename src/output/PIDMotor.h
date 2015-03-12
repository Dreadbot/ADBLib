#pragma once

#include <WPILib.h>
#include "SimpleMotor.h"

namespace ADBLib
{
	class PIDMotor : public SimpleMotor
	{
	public:
		enum PIDK {Kp, Ki, Kd};

		PIDMotor();
		PIDMotor(double P, double I, double D, PIDSource* newSource);
		~PIDMotor();

		//SimpleMotor overrides
		void set(float value); //!< PID-enabled set function. Does NOT work without PID stuff being properly set up!
		void setPWMMotor(Talon* motor); //!< Sets this PIDMotor's output as a non-CAN talon.
		void setCANMotor(CANTalon* motor); //!< Sets this PIDMotor's output as a CAN talon.

		//Useful PID things from the PIDController class
		float getError(); //!< Returns the current error.
		bool isOnTarget(); //!< Is the motor on target?
		void setAbsToler(float tolerance); //!< Sets the absolute value tolerance.
		void setContinuous(bool newCont); //!< Sets whether this PID motor is continuous or no. See documentation for PIDController in WPILib
		void setInputRange(float newMax, float newMin); //!< Sets the input range expected from the sensor.
		void setOutputRange(float newMax, float newMin); //!< Sets the output range that the output expects
		void setPercentTolerance(float newPToler); //!< Sets the percent tolerance of the PID controller..

		//PIDMotor-specific stuff
		void setK(double newVal, PIDK slot); //!< Sets a PID constant.
		void setPID(double newP, double newI, double newD); //!< Sets ALL the PID constants!
		void setPeriod(float newPeriod); //!< Sets the update period (?)
		void setSource(PIDSource* newSource); //!< Sets the PID input source. This should be a sensor.

		double getK(PIDK slot); //!< Gets a PID constant.
		float getPeriod(); //!< Gets the sensor update period.
	protected:
		void setupCtrl(); //!< Internal function; creates a new PID since the only way to set a bunch of stuff is through the constructor for PIDController

		PIDController* pidctrl;
		double PIDValues[3];
		float period;
		PIDSource* source;
	};
}
