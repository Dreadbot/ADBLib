#include "MotorGrouping.h"

namespace ADBLib
{
	MotorGroup::MotorGroup()
	{
		name = "Not a MotorGroup";
	}
	void MotorGroup::set(float value)
	{
		for (auto iter = motors.begin(); iter != motors.end(); iter++)
			(*iter)->set(value);
	}
	void MotorGroup::addMotor(SimpleMotor* newMotor)
	{
		motors.push_back(newMotor);
	}
	void MotorGroup::setName(string newName)
	{
		name = newName;
	}
	string MotorGroup::getName()
	{
		return name;
	}
}
