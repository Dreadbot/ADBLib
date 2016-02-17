#include "SimplePID.h"
#include "Notifier.h"
#include <cmath>

namespace ADBLib
{
	/**
	 * Create a PID object with the given constants for P, I, D.
	 * Default values for min/max IO are -1 and 1 respectively, continuous
	 * input is disabled, tolerance is set to 5%, and the SimplePID starts
	 * as disabled (zero output).
	 * @param Kp The proportional coefficient
	 * @param Ki The integral coefficient
	 * @param Kd The derivative coefficient
	 * @param period The loop time for doing calculations in seconds. This
	 * particularly affects calculations of the integral and differential
	 * terms. The default is 50 ms.
	 */
	SimplePID::SimplePID(double Kp, double Ki, double Kd, bool continuous, double period)
	{
		m_controlLoop = new Notifier(SimplePID::callCalculate, this);
		m_P = Kp;
		m_I = Ki;
		m_D = Kd;
		m_maximumOutput = 1.0;
		m_minimumOutput = -1.0;
		m_maximumInput = 1.0;
		m_minimumInput = -1.0;
		m_continuous = false;
		m_enabled = false;
		m_setpoint = 0.0;
		m_error = 0.0;
		m_prevError = 0.0;
		m_totalError = 0.0;
		m_tolerance = 0.05;
		m_result = 0.0;
		m_period = period;
		m_measuredValue = 0.0;
		m_controlLoop->StartPeriodic(m_period);
	}

	/**
	 * Destroys this object.
	 */
	SimplePID::~SimplePID()
	{
		delete m_controlLoop;
	}

	/**
	 * Internal function. Calls the calculate function... for some reason.
	 * @param controller A pointer to a SimplePID object.
	 */
	void SimplePID::callCalculate(void *controller)
	{
		SimplePID *control = (SimplePID*) controller;
		control->calculate();
	}

	/**
	 * Internal function. Performs necessary PID calculations.
	 */
	void SimplePID::calculate()
	{
		if (m_enabled)
		{
			double input = m_measuredValue;
			m_error = m_setpoint - input;
			if (m_continuous)
			{
				if (fabs(m_error) > (m_maximumInput - m_minimumInput) / 2)
				{
					if (m_error > 0)
						m_error = m_error - m_maximumInput + m_minimumInput;
					else
						m_error = m_error + m_maximumInput - m_minimumInput;
				}
			}
			if (((m_totalError + m_error) * m_I < m_maximumOutput)
					&& ((m_totalError + m_error) * m_I > m_minimumOutput))
				m_totalError += m_error;
			m_result = m_P * m_error + m_I * m_totalError
					+ m_D * (m_error - m_prevError);
			m_prevError = m_error;
			if (m_result > m_maximumOutput)
				m_result = m_maximumOutput;
			else if (m_result < m_minimumOutput)
				m_result = m_minimumOutput;
		}
	}

	/**
	 * Sets PID constants.
	 * @param p The proportional constant. Multiplied by the error.
	 * @param i The integral constant. Multiplied by the sum of error.
	 * @param d The derivative constant. Multiplied by the rate at which error is changing.
	 */
	void SimplePID::setPID(double p, double i, double d)
	{
		m_P = p;
		m_I = i;
		m_D = d;
	}

	/**
	 * Gets the output as calculated using input and setpoint.
	 * @return The output from the PID calculation. Give this to the object you want work with (eg. a motor).
	 */
	double SimplePID::getOutput()
	{
		return m_result;
	}

	/**
	 * Sets the measured value, presumably from sensors.
	 * @param mv The measured value as a double.
	 */
	void SimplePID::setMV(double mv)
	{
		m_measuredValue = mv;
	}

	/**
	 * Sets the input range this object expects. For example. a gyro could
	 * output from -180 to 180, so you would call this function with Set
	 * @param minimumInput The minimum input expected. Can be negative.
	 * @param maximumInput The maximum input expected.
	 * @todo Investigate "SetSetpoint(m_setpoint)"
	 */
	void SimplePID::setInputRange(double minimumInput, double maximumInput)
	{
		m_minimumInput = minimumInput;
		m_maximumInput = maximumInput;
		setSetpoint(m_setpoint);
	}

	/**
	 * Sets the output range delivered. For example, if you have a drivebase
	 * expecting values between -512 and 512, call this function with -512
	 * and 512, respectively.
	 * @param minimumOutput The minimum output delivered. Can be negative.
	 * @param maximumOutput The maximum output delivered.
	 */
	void SimplePID::setOutputRange(double minimumOutput, double maximumOutput)
	{
		m_minimumOutput = minimumOutput;
		m_maximumOutput = maximumOutput;
	}

	/**
	 * Sets the point that PID calculations will try to work towards.
	 * This value should be somewhere within the output range, which
	 * by default is from -1 to 1. This function can and should be
	 * used similarly to motor set functions.
	 * @param setpoint The desired setpoint.
	 */
	void SimplePID::setSetpoint(double setpoint)
	{
		if (m_maximumInput > m_minimumInput)
		{
			if (setpoint > m_maximumInput)
				m_setpoint = m_maximumInput;
			else if (setpoint < m_minimumInput)
				m_setpoint = m_minimumInput;
			else
				m_setpoint = setpoint;
		}
		else
			m_setpoint = setpoint;
	}

	/**
	 * Sets the tolerance to error.
	 * @param tolerance Tolerance as a percent (eg. 0.02)
	 */
	void SimplePID::setTolerance(double tolerance)
	{
		m_tolerance = tolerance;
	}

	/**
	 * Returns the status of whether this PID controller is on target or not to
	 * within tolerance set by the SetTolerance function. Tolerance is set at 5%
	 * by default. Calculation is done by getting the absolute value of error and
	 * checking whether it's less than the tolerance times the input range.
	 * @return True for on-target, false otherwise.
	 */
	bool SimplePID::onTarget()
	{
		return (fabs(m_error) < m_tolerance * (m_maximumInput - m_minimumInput));;
	}

	/**
	 * Enables the SimplePID, starting output from getOutput().
	 */
	void SimplePID::enable()
	{
		m_enabled = true;
	}

	/**
	 * Disables the SimplePID, zeroing output from getOutput().
	 */
	void SimplePID::disable()
	{
		m_enabled = false;
	}

	/**
	 * Resets the SimplePID, disabling it and resetting all
	 * error and calculated output.
	 */
	void SimplePID::reset()
	{
		disable();
		m_prevError = 0;
		m_totalError = 0;
		m_result = 0;
	}
}
