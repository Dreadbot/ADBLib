#pragma once
#include <WPILib.h>
#include <vector>
#include <string.h>
#include "SimpleMotor.h"
using std::vector;
using std::string;

namespace ADBLib
{
	class MotorGroup
	{
	public:
		MotorGroup();
		void set(float value);
		void addMotor(SimpleMotor* newMotor);

		void setName(string newName);
		string getName();
	protected:
		vector<SimpleMotor*> motors;
		string name;
	};
}
