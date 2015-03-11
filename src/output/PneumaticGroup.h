#pragma once
#include <WPILib.h>
#include <vector>
#include <string>
#include "SimplePneumatic.h"

using std::string;
using std::vector;

namespace ADBLib
{
	class PneumaticGroup
	{
	public:
		PneumaticGroup();
		void set(float value);
		void addPneumatic(SimplePneumatic* newPneumatic);

		void setName(string newName);
		string getName();
	protected:
		vector<SimplePneumatic*> pneumatics;
		string name;
	};
}
