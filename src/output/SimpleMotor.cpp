#include "SimpleMotor.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; invert off, type = pwm, nullptrs for CANMotor and PWMMotor and sets the motor as enabled.
	 */
	SimpleMotor::SimpleMotor()
	{
		invert = false;
		type = PWM;
		CANMotor = nullptr;
		PWMMotor = nullptr;
		enabled = true;
	}

	/**
	 * @brief Smart set - automatically handles for invert, CAN/PWM controls. Give it a value and go.
	 * @param value A value from -1 to 1.
	 */
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

	/**
	 * @brief Switches this motor over to being a CAN motor.
	 * @param motor A pointer to a CANTalon.
	 */
	void SimpleMotor::setCANMotor(CANTalon* motor)
	{
		if (motor == nullptr)
			return; //Break for invalid motors.

		type = CAN;
		PWMMotor = nullptr;
		CANMotor = motor;

		CANMotor->SetControlMode(CANSpeedController::ControlMode::kPercentVbus); //Accepts values from 1 to -1
	}

	/**
	 * @brief Switches this motor over to being a PWM motor.
	 * @param motor A pointer to a Talon.
	 */
	void SimpleMotor::setPWMMotor(Talon* motor)
	{
		if (motor == nullptr)
			return;

		type = PWM;
		PWMMotor = motor;
		CANMotor = nullptr;
	}

	/**
	 * @brief Sets whether this motor is inverted or not.
	 * @param newInvert True for inverted, false otherwise.
	 */
	void SimpleMotor::setInvert(bool newInvert)
	{
		invert = newInvert;
	}

	/**
	 * @brief Gets whether this motor is inverted or not.
	 * @return True if inverted, false otherwise.
	 */
	bool SimpleMotor::getInvert()
	{
		return invert;
	}

	/**
	 * @brief Enables the motor.
	 */
	void SimpleMotor::enable()
	{
		if (type == CAN)
			CANMotor->EnableControl();
		enabled = true;
	}

	/**
	 * @brief Disables the motor.
	 */
	void SimpleMotor::disable()
	{
		if (type == CAN)
			CANMotor->Disable();
		enabled = false;
	}

	/**
	 * @brief Sets the rate at which voltage to this motor will change.
	 * @param rate A value. Unknown range.
	 * @note Only works for CAN motors!
	 */
	void SimpleMotor::setVoltageRampRate(float rate)
	{
		if (type == CAN)
			CANMotor->SetVoltageRampRate(rate);
	}
}
