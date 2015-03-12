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
		PIDMotor(double P, double I, double D);
		~PIDMotor();

		//SimpleMotor overrides
		void set(float value);
		void setPWMMotor(Talon* motor);
		void setCANMotor(CANTalon* motor);

		//Useful PID things from the PIDController class
		float getError();
		bool isOnTarget();
		void setAbsToler(float tolerance);
		void setContinuous(bool newCont);
		void setInputRange(float newMax, float newMin);
		void setOutputRange(float newMax, float newMin);
		void setPercentTolerance(float newPToler);

		//PIDMotor-specific stuff
		void setK(double newVal, PIDK slot);
		void setPID(double newP, double newI, double newD);
		void setPeriod(unsigned float newPeriod);
		void setSource(PIDSource* source);

		double getK(PIDK slot);
		float getPeriod();
	protected:
		PIDController* pidctrl;
		double PIDValues[3];
		unsigned float period;
	};
}
