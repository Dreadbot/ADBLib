#pragma once
#include <WPILib.h>

namespace ADBLib
{
	/**
	 * Class for doing PID calculations without slaving an output into a PIDController
	 * object. Class originally by Parker Stebbins, adapted for use in ADBLib in 2016.
	 *
	 * To use this class, first give it your PID values, select whether input is
	 * continuous, and select a control period (default is 50 ms). To provide sensor
	 * input, call setMV() and provide your sensor input. To set a setpoint, use
	 * setSetpoint(). To get the calculated value that your output should be set to,
	 * call getOutput().
	 * @note You MUST call enable(), all SimplePID objects start as DISABLED!
	 */
	class SimplePID
	{
		public:
			SimplePID(double p, double i, double d, bool continuous, double period = 0.05);
			~SimplePID();
			double getOutput();
			void setInputRange(double minimumInput, double maximumInput);
			void setOutputRange(double mimimumOutput, double maximumOutput);
			void setPID(double p, double i, double d);
			void setMV(double mv);
			void setSetpoint(double setpoint);
			double setSetpoint();
			double setError();
			void setTolerance(double percent);
			bool onTarget();
			void enable();
			void disable();
			void reset();
		protected:
			double m_P;		// factor for "proportional" control
			double m_I;		// factor for "integral" control
			double m_D;		// factor for "derivative" control
			double m_maximumOutput;	// |maximum output|
			double m_minimumOutput;	// |minimum output|
			double m_maximumInput;	// maximum input - limit setpoint to this
			double m_minimumInput;	// minimum input - limit setpoint to this
			bool m_continuous;	// do the endpoints wrap around? eg. Absolute encoder
			bool m_enabled; 	// is the pid controller enabled
			double m_prevError;	// the prior sensor input (used to compute velocity)
			double m_totalError; 	// the sum of the errors for use in the integral calc
			double m_tolerance;	// the percentage error that is considered on target
			double m_setpoint;
			double m_error;
			double m_result;
			double m_period;
			double m_measuredValue; // the last value recorded from the measurement device.
			Notifier *m_controlLoop;
			static void callCalculate(void *controller); //TODO: Exterminate "callCalculate" function if possible
			void calculate();
	};
}
