#include "PIDMotor.h"

namespace ADBLib
{
	/**
	 * @brief Sets up a PID motor with a given speed controller and an input source.
	 * @param newMotor A SpeedController. Give it a motor, all motors inherit SpeedController.
	 * @param newSrc The input source. Give it a sensor, all sensors inherit PIDSource.
	 */
	PIDMotor::PIDMotor(SpeedController* newMotor, PIDSource* newSrc)
	{
		pidctrl = nullptr;
		source = newSrc;
		motor = newMotor;
		period = 0.05; //50 milliseconds
		PIDValues[Kp] = 1.f;
		PIDValues[Ki] = 0.f;
		PIDValues[Kd] = 0.f;
		setupCtrl();
	}

	/**
	 * @brief Sets up a PID motor with a given speed controller, an input source, and PID values.
	 * @param P The proportional coefficient.
	 * @param I The integral coefficient.
	 * @param D The derivative coefficient.
	 * @param newMotor A motor as a speed control.
	 * @param newSource Practically any given sensor.
	 */
	PIDMotor::PIDMotor(double P, double I, double D, PIDOutput* newMotor, PIDSource* newSrc)
	{
		pidctrl = nullptr;
		source = newSrc;
		motor = newMotor;
		period = 0.05; //50 milliseconds;
		PIDValues[Kp] = P;
		PIDValues[Ki] = I;
		PIDValues[Kd] = D;
		setupCtrl();
	}

	PIDMotor::~PIDMotor()
	{
		if (pidctrl != nullptr)
		{
			delete pidctrl;
			pidctrl = nullptr;
		}
		delete motor; //These are *technically* abstract, but
		delete source;//no-one is going to GIVE these an abstract object...

	}

	/**
	 * @@brief PID-enabled set function. Typically used for speed.
	 * @param value The speed as a float, range from -1 to 1.
	 * @todo Needs testing, especially for position
	 */
	void PIDMotor::set(float value)
	{
		if (pidctrl == nullptr)
			setupCtrl();

		if (!enabled)
			return;

		//Standard invert
		if (invert)
			value = -value;

		pidctrl->SetSetpoint(value * 1023.f); //This should work. Should.
	}

	/**
	 * @brief Get the error from the pid controller.
	 * @return The error. Range unknown.
	 * @todo find the range of the output for this function
	 */
	float PIDMotor::getError()
	{
		if (pidctrl != nullptr)
			return pidctrl->GetError();
		else
			return -1; //Indicator of error
	}

	/**
	 * @brief Based on the tolerance and error, determine if the motor is on target.
	 * @return True for on-target, false for off-target.
	 */
	bool PIDMotor::isOnTarget()
	{
		if (pidctrl != nullptr)
			return pidctrl->OnTarget();
		else return false;
	}

	/**
	 * @brief Sets the absolute value of the tolerance.
	 * @param tolerance Tolerance as a percent (presumably range 0-1)
	 */
	void PIDMotor::setAbsToler(float tolerance)
	{
		if (pidctrl != nullptr)
			pidctrl->SetAbsoluteTolerance(tolerance);
	}

	/**
	 * @brief See documentation for same function in PIDController in WPILib
	 */
	void PIDMotor::setContinuous(bool newCont)
	{
		if (pidctrl != nullptr)
			pidctrl->SetContinuous(newCont);
	}

	/**
	 * @brief Sets the input range expected from the sensor.
	 * @param newMax The new maximum value.
	 * @param newMin The new minimum value.
	 */
	void PIDMotor::setInputRange(float newMax, float newMin)
	{
		if (pidctrl != nullptr)
			pidctrl->SetInputRange(newMin, newMax);
	}

	/**
	 * @brief Sets the output range that the output expects.
	 * @param newMax The new maximum value.
	 * @param newMin The new minimum value.
	 * @note You probably don't need to touch this function.
	 */
	void PIDMotor::setOutputRange(float newMax, float newMin)
	{
		if (pidctrl != nullptr)
			pidctrl->SetOutputRange(newMin, newMax);
	}

	/**
	 * @brief Sets the percent tolerance of the PID controller.
	 * @param newPToler The new percent tolerance, range 0-100
	 * @todo Verify this range
	 */
	void PIDMotor::setPercentTolerance(float newPToler)
	{
		if (pidctrl != nullptr)
			pidctrl->SetPercentTolerance(newPToler);
	}

	/**
	 * @brief Sets ALL the PID constants.
	 * @param P The proportional coefficient.
	 * @param I The integral coefficient.
	 * @param D The derivative coefficient.
	 */
	void PIDMotor::setPID(double newP, double newI, double newD)
	{
		PIDValues[Kp] = newP;
		PIDValues[Ki] = newI;
		PIDValues[Kd] = newD;

		if (pidctrl != nullptr)
			pidctrl->SetPID(PIDValues[Kp], PIDValues[Ki], PIDValues[Kd]);
	}

	/**
	 * @brief Set the... control period?
	 * @param newPeriod The new control period?
	 * @todo verify this is actually anything to do with the period.
	 */
	void PIDMotor::setPeriod(float newPeriod)
	{
		period = newPeriod; //Wait, what?
		setupCtrl();
	}

	/**
	 * @brief Set a new input source (sensor).
	 * @param newSource A new input source. Can be most sensors.
	 */
	void PIDMotor::setSource(PIDSource* newSource)
	{
		source = newSource;
		setupCtrl(); //This is a major change. Reset the PIDController.
	}

	/**
	 * @brief Internal function only. Re-sets-up the PID Controller.
	 */
	void PIDMotor::setupCtrl()
	{
		if (pidctrl != nullptr)
		{
			delete pidctrl;
			pidctrl = nullptr; //KILL IT WITH FIRE!
		}

		pidctrl = new PIDController(
				PIDValues[Kp],
				PIDValues[Ki],
				PIDValues[Kd],
				source,
				motor,
				period);
	}
}
