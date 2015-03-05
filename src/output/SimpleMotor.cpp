#include "SimpleMotor.h"

namespace ADBLib
{
	SimpleMotor::SimpleMotor()
	{
		invert = false;
		type = PWM;
		CANMotor = nullptr;
		PWMMotor = nullptr;
	}
	void SimpleMotor::set(float value)
	{
		//Account for inverts
		if (invert)
			value = !value;

		if (type == CAN && CANMotor != nullptr)
			CANMotor->Set(value);
		if (type == PWM && PWMMotor != nullptr)
			PWMMotor->Set(value);
	}
	void SimpleMotor::setCANMotor(CANTalon* motor)
	{
		if (motor == nullptr)
			return; //Break for invalid motors.

		type = CAN;
		PWMMotor = nullptr;
		CANMotor = motor;
	}
	void SimpleMotor::setPWMMotor(Talon* motor)
	{
		if (motor == nullptr)
			return;

		type = PWM;
		PWMMotor = motor;
		CANMotor = nullptr;
	}
	void SimpleMotor::setInvert(bool newInvert)
	{
		invert = newInvert;
	}
	bool SimpleMotor::getInvert()
	{
		return invert;
	}
	SimpleMotor::MotorType SimpleMotor::getType()
	{
		return type;
	}
}
