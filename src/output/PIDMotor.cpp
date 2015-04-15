#include "PIDMotor.h"

namespace ADBLib
{
	PIDMotor::PIDMotor()
	{
		pidctrl = nullptr;
		source = nullptr;
		period = 0.05; //50 milliseconds
	}
	PIDMotor::PIDMotor(double P, double I, double D, PIDSource* newSource)
	{
		pidctrl = nullptr;
		source = nullptr;
		period = 0.05; //50 milliseconds;

		PIDValues[Kp] = P;
		PIDValues[Ki] = I;
		PIDValues[Kd] = D;
		source = newSource;
	}
	PIDMotor::~PIDMotor()
	{
		if (pidctrl != nullptr)
		{
			delete pidctrl;
			pidctrl = nullptr;
		}

	}
	void PIDMotor::set(float value)
	{
		if (pidctrl == nullptr)
			setupCtrl();

		if (!enabled)
			return;

		//Standard invert
		if (invert)
			value = -value;

		pidctrl->SetSetpoint(value); //This should work. Should.
	}
	void PIDMotor::setPWMMotor(Talon* motor)
	{
		if (motor == nullptr)
			return;

		type = PWM;
		PWMMotor = motor;
		CANMotor = nullptr;

		setupCtrl();
	}
	void PIDMotor::setCANMotor(CANTalon* motor)
	{
		if (motor == nullptr)
			return;

		type = CAN;
		PWMMotor = nullptr;
		CANMotor = motor;

		setupCtrl();
	}
	float PIDMotor::getError()
	{
		if (pidctrl != nullptr)
			return pidctrl->GetError();
		else
			return -1; //Indicator of error
	}
	bool PIDMotor::isOnTarget()
	{
		if (pidctrl != nullptr)
			return pidctrl->OnTarget();
		else return false;
	}
	void PIDMotor::setAbsToler(float tolerance)
	{
		if (pidctrl != nullptr)
			pidctrl->SetAbsoluteTolerance(tolerance);
	}
	void PIDMotor::setContinuous(bool newCont)
	{
		if (pidctrl != nullptr)
			pidctrl->SetContinuous(newCont);
	}
	void PIDMotor::setInputRange(float newMax, float newMin)
	{
		if (pidctrl != nullptr)
			pidctrl->SetInputRange(newMin, newMax);
	}
	void PIDMotor::setOutputRange(float newMax, float newMin)
	{
		if (pidctrl != nullptr)
			pidctrl->SetOutputRange(newMin, newMax);
	}
	void PIDMotor::setPercentTolerance(float newPToler)
	{
		if (pidctrl != nullptr)
			pidctrl->SetPercentTolerance(newPToler);
	}
	void PIDMotor::setK(double newVal, PIDK slot)
	{
		PIDValues[3] = newVal;

		if (pidctrl != nullptr)
			pidctrl->SetPID(PIDValues[Kp], PIDValues[Ki], PIDValues[Kd]);
	}
	void PIDMotor::setPID(double newP, double newI, double newD)
	{
		PIDValues[Kp] = newP;
		PIDValues[Ki] = newI;
		PIDValues[Kd] = newD;

		if (pidctrl != nullptr)
			pidctrl->SetPID(PIDValues[Kp], PIDValues[Ki], PIDValues[Kd]);
	}
	void PIDMotor::setPeriod(float newPeriod)
	{
		period = newPeriod; //Wait, what?
	}
	void PIDMotor::setSource(PIDSource* newSource)
	{
		source = newSource;
		setupCtrl(); //This is a major change. Reset the PIDController.
	}
	double PIDMotor::getK(PIDK slot)
	{
		return PIDValues[slot];
	}
	float PIDMotor::getPeriod()
	{
		return period;
	}
	void PIDMotor::setupCtrl()
	{
		if (pidctrl != nullptr)
		{
			delete pidctrl;
			pidctrl = nullptr; //KILL IT WITH FIRE!
		}

		if (CANMotor == nullptr && PWMMotor == nullptr)
			return; //There is no motor; nothing further can be done.
		if (source == nullptr)
			return; //What, you want PID without a source? NO!

		PIDOutput* output;
		if (type == CAN)
			output = CANMotor;
		else
			output = PWMMotor;

		pidctrl = new PIDController(
				PIDValues[Kp],
				PIDValues[Ki],
				PIDValues[Kd],
				source,
				output,
				period);

	}
}
