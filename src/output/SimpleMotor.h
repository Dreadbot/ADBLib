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
		SimpleMotor(Talon* newTalon);
		SimpleMotor(CANTalon* newCANTalon);
		virtual ~SimpleMotor() {};
		virtual void set(float value);
		virtual void setPWMMotor(Talon* motor);
		virtual void setCANMotor(CANTalon* motor);
		void setInvert(bool newInvert);
		void enable();
		void disable();
		void setVoltageRampRate(float rate);

		bool getInvert();
	protected:
		MotorType type;
		bool invert;
		bool enabled;
		CANTalon* CANMotor;
		Talon* PWMMotor;
	};
}
