#include "SimplePneumatic.h"

namespace ADBLib
{
	/**
	 * @brief Constructor.
	 */
	SimplePneumatic::SimplePneumatic()
	{
		type = single;
		invert = false;
		dPneumatic = nullptr;
		sPneumatic = nullptr;
	}

	/**
	 * @brief Constructor; configures this pneumatic as a double solenoid.
	 * @param newDouble A pointer to a DoubleSolenoid object.
	 */
	SimplePneumatic::SimplePneumatic(DoubleSolenoid* newDouble) : SimplePneumatic()
	{
		setDouble(newDouble);
	}

	/**
	 * @brief Constructor; configures this pneumatic as a single solenoid.
	 * @param newSingle A pointer to a solenoid object.
	 */
	SimplePneumatic::SimplePneumatic(Solenoid* newSingle) : SimplePneumatic()
	{
		setSingle(newSingle);
	}

	/**
	 * @brief Sets the value for this pneumatic; if it's a single solenoid, not kStop values are automatically adjusted.
	 * @param value A DoubleSolenoid::Value as defined by DoubleSolenoid
	 */
	void SimplePneumatic::set(DoubleSolenoid::Value value)
	{
		if (type == dual)
		{
			if (invert)
			{
				if (value == DoubleSolenoid::kReverse)
					value = DoubleSolenoid::kForward;
				else if (value == DoubleSolenoid::kForward)
					value = DoubleSolenoid::kReverse;
			}
			if (dPneumatic != nullptr)
				dPneumatic->Set(value);
		}
		else
		{
			bool input = (bool)value;
			if (invert)
				input = !input;
			if (sPneumatic != nullptr)
				sPneumatic->Set(input);
		}
	}

	/**
	 * @brief Smart set -- automatically converts from an integer to a DoubleSolenoid::Value.
	 * @param dir The direction. Negative is reverse, 0 is off, and positive is forward.
	 */
	void SimplePneumatic::set(int dir)
	{
		if (dir < 0)
			set(DoubleSolenoid::kReverse);
		else if (dir > 0)
			set(DoubleSolenoid::kForward);
		else
			set(DoubleSolenoid::kOff);
	}

	/**
	 * @brief Sets the solenoid as a double solenoid.
	 * @param newDouble A pointer to a DoubleSolenoid.
	 */
	void SimplePneumatic::setDouble(DoubleSolenoid* newDouble)
	{
		if (newDouble == nullptr)
			return;

		type = dual;
		sPneumatic = nullptr;
		dPneumatic = newDouble;
	}

	/**
	 * @brief Sets the solenoid as a single solenoid.
	 * @param newSingle A pointer to a Solenoid.
	 */
	void SimplePneumatic::setSingle(Solenoid* newSingle)
	{
		if (newSingle == nullptr)
			return;

		type = single;
		sPneumatic = newSingle;
		dPneumatic = nullptr;
	}
}

