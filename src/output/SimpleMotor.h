#pragma once
#include <WPILib.h>

/*
 * \brief A simple motor class that allows for control of a motor, regardless of what type it is.
 */
namespace ADBLib
{
	class SimpleMotor
	{
	public:
		enum MotorType {CAN, PWM};

		SimpleMotor();
		virtual void set(float value); //!< Smart set - automatically handles for invert, CAN/PWM controls. Give it a value and go.
		void setPWMMotor(Talon* motor); //!< Sets a PWM motor, removing the CAN motor in the process. Checks for nullptrs.
		void setCANMotor(CANTalon* motor); //!< Sets a CAN motor, removing the PWM motor in the process. Checks for nullptrs.
		void setInvert(bool newInvert); //!< Sets an invert that is automatically handled for in set().
		void enable(); //!< Enables the motor.
		void disable(); //!< Disables the motor.
		void setVoltageRampRate(float rate);

		MotorType getType(); //!< Returns the type of the motor - CAN or PWM.
		bool getInvert(); //!< Returns if the motor is inverted. True is invert, false is no invert.
	protected:
		MotorType type;
		bool invert;
		bool enabled;
		CANTalon* CANMotor;
		Talon* PWMMotor;
	};
}
