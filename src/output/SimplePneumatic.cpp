#include "SimplePneumatic.h"

namespace ADBLib
{
	SimplePneumatic::SimplePneumatic()
	{
		type = single;
		invert = false;
		dPneumatic = nullptr;
		sPneumatic = nullptr;
	}
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
	void SimplePneumatic::set(int dir)
	{
		if (dir < 0)
			set(DoubleSolenoid::kReverse);
		else if (dir > 0)
			set(DoubleSolenoid::kForward);
		else
			set(DoubleSolenoid::kOff);
	}
	void SimplePneumatic::setDouble(DoubleSolenoid* newDouble)
	{
		if (newDouble == nullptr)
			return;

		type = dual;
		sPneumatic = nullptr;
		dPneumatic = newDouble;
	}
	void SimplePneumatic::setSingle(Solenoid* newSingle)
	{
		if (newSingle == nullptr)
			return;

		type = single;
		sPneumatic = newSingle;
		dPneumatic = nullptr;
	}
	SimplePneumatic::PneumaticType SimplePneumatic::getType()
	{
		return type;
	}
}
