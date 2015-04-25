#include "PneumaticGroup.h"

namespace ADBLib
{
	PneumaticGroup::PneumaticGroup()
	{
		name = "Not a PneumaticGroup";
	}
	void PneumaticGroup::set(float value)
	{
		for (auto iter = pneumatics.begin(); iter != pneumatics.end(); iter++)
			(*iter)->set(value);
	}
	void PneumaticGroup::addPneumatic(SimplePneumatic* newPneumatic)
	{
		if (newPneumatic != nullptr)
			pneumatics.push_back(newPneumatic);
	}
	void PneumaticGroup::setName(string newName)
	{
		name = newName;
	}
	string PneumaticGroup::getName()
	{
		return name;
	}
}
