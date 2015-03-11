#pragma once
#include <WPILib.h>
#include <vector>
#include <string.h>
#include "SimpleMotor.h"

using std::vector;
using std::string;

namespace ADBLib
{
	class MotorGroup //!< Simple motor grouping class. Allows control over multiple motors without additional code.
	{
	public:
		MotorGroup();
		void set(float value); //!< Pass the given value to all motors in this motor group.
		void addMotor(SimpleMotor* newMotor); //!< Add a new motor to the motor group.

		void setName(string newName); //!< Sets the name of this motor group.
		string getName(); //!< Gets the name of this motor group.
	protected:
		vector<SimpleMotor*> motors;
		string name;
	};
}
