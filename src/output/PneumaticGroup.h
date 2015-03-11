#pragma once
#include <WPILib.h>
#include <vector>
#include <string>
#include "SimplePneumatic.h"

using std::string;
using std::vector;

namespace ADBLib
{
	class PneumaticGroup //!< Simple pneumatic grouping class. Allows control over multiple pneumatics without additional code.
	{
	public:
		PneumaticGroup();
		void set(float value); //!< Pass the given value to all the pneumatics in this group.
		void addPneumatic(SimplePneumatic* newPneumatic); //!< Add a new pneumatic to this pneumatic group.

		void setName(string newName); //!< Sets the name of this pneumatic group.
		string getName(); //!< Gets the name of this pneumatic group.
	protected:
		vector<SimplePneumatic*> pneumatics;
		string name;
	};
}
