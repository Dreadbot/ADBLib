#pragma once
#include <WPILib.h>
#include "Drivebase.h"

namespace ADBLib
{
	/*
	 * \brief Simple omni drive class. Supports one central motor for translational controls.
	 */
	class OmniDrive : public Drivebase
	{
	public:
		OmniDrive();
		void drive(float x = 0.0, float y = 0.0, float r = 0.0);
		void setTransMotor(SimpleMotor* newMotor); //!< Sets the central motor used for x translation.
	protected:
		SimpleMotor* transMotor;
	};
}
