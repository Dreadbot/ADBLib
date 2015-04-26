#include "SimpleMotor.h"

namespace ADBLib
{
	SimpleMotor::SimpleMotor()
	{
		invert = false;
		type = PWM;
		CANMotor = nullptr;
		PWMMotor = nullptr;
		enabled = true;
	}
	SimpleMotor::~SimpleMotor()
	{
		//Nope
	}
	void SimpleMotor::set(float value)
	{
		if (!enabled)
			return;

		//Account for inverts
		if (invert)
			value = !value;

		if (type == CAN && CANMotor != nullptr)
			CANMotor->Set(value, (uint8_t)0x00);
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

		CANMotor->SetControlMode(CANSpeedController::ControlMode::kPercentVbus); //Accepts values from 1 to -1
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
	void SimpleMotor::enable()
	{
		if (type == CAN)
			CANMotor->EnableControl();
		enabled = true;
	}
	void SimpleMotor::disable()
	{
		if (type == CAN)
			CANMotor->Disable();
		enabled = false;
	}
	void SimpleMotor::setVoltageRampRate(float rate)
	{
		if (type == CAN)
			CANMotor->SetVoltageRampRate(rate);
	}
}
