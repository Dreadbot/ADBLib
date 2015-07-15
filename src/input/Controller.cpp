#include "Controller.h"

namespace ADBLib
{
	/**
	 * @brief Initializes the Controller object with no joystick; you will need to use setJoystick to set the joystick if you use this constructor.
	 */
	Controller::Controller()
	{
		joystick = nullptr;
		for (int i = 0; i < 15; ++i)
		{
			cooldownTimes[i] = 0.f;
			axisRanges[i][0] = -1.0;
			axisRanges[i][1] = 1.0;
			toggles[i] = false;
		}
	}

	/**
	 * @brief Initializes the controller object with a pre-set Joystick.
	 * @param newJoystick A shared_ptr for a joystick object. If this joystick is only in use here, use std::make_shared
	 */
	Controller::Controller(std::shared_ptr<Joystick> newJoystick)
	{
		joystick = newJoystick;
		for (int i = 0; i < 15; ++i)
		{
			cooldownTimes[i] = 0.f;
			axisRanges[i][0] = -1.0;
			axisRanges[i][1] = 1.0;
		}
	}

	/**
	 * @brief Returns the state of the button as a boolean. Automatically handles for button cooldown.
	 * @param ID The ID of the button. Button ID changes depending on controller mode (X-Mode or D-Mode).
	 * @param cooldown The desired cooldown, in seconds. The cooldown will not be changed if a previous cooldown is still active.
	 */
	bool Controller::getButton(int ID, float cooldown)
	{
		if (ID >= 15 || ID <= 0)
			return false; //Out-of-bounds IDs not accepted.
		if (cooldownTimers[ID].Get() < cooldownTimes[ID])
			return false; //Actual cooldown handling

		if (joystick->GetRawButton(ID) == true)
		{
			cooldownTimes[ID] = cooldown;
			cooldownTimers[ID].Reset();
			cooldownTimers[ID].Start();
			return true;
		}
		return false;
	}

	/**
	* @brief Returns the state of the button as a boolean. Changes state based on input from the button, with cooldown taken into account.
	* @param ID The ID of the button. Button ID changes depending on controller mode (X_Mode or D-Mode).
	* @param cooldown The desired cooldown, in seconds. The cooldown will not be changed if a previous cooldown is still active.
	*/
	bool Controller::getButtonToggle(int ID, float cooldown)
	{
		if (getButton(ID, cooldown))
			toggles[ID] = !toggles[ID];
		return toggles[ID];
	}

	/**
	 * @brief Gets the raw button setting as true or false, disregarding any active cooldowns.
	 * @param ID The ID of the button. Button changes depending on controller mode (X-Mode or D-Mode)
	 */
	bool Controller::getButtonRaw(int ID)
	{
		if (ID >= 15 || ID <= 0)
			return false; //Out-of-bounds IDs not accepted.
		return joystick->GetRawButton(ID);
	}

	/**
	 * @brief Get the state of the joystick axis. Returns values that depend on your set max/min output settings.
	 * @param ID The ID of the axis.
	 */
	double Controller::getAxis(int ID)
	{
		if (ID > 15 || ID < 0)
			return 0;

		double delta = axisRanges[ID][1] - axisRanges[ID][0]; //max - min
		double input = joystick->GetRawAxis(ID) + 1.0; //Now on a scale from 0-2.
		return (input * delta) + axisRanges[ID][0];
	}

	/**
	 * @brief Sets the input range for a specific axis.
	 * @param axisID The ID of the axis whose range is being modified.
	 * @param max The new maximum input.
	 * @param min The new minimum input.
	 */
	void Controller::setInputRange(int axisID, float max, float min)
	{
		if (axisID >= 15 || axisID <= 0)
			return;
		axisRanges[axisID][0] = min;
		axisRanges[axisID][1] = max;
	}

	/**
	 * @brief Sets the joystick to get data from.
	 * @param newJoystick A shared pointer to the joystick to use.
	 */
	void Controller::setJoystick(std::shared_ptr<Joystick> newJoystick)
	{
		joystick = newJoystick;
	}

	/**
	 * @brief Make the controller rumble using built-in rumbler thingies.
	 * @param side The side on which the joystick will rumble. Definedby WPILib.
	 * @param intensity The intensity of vibration on a scale from 0 to 1.
	 */
	void Controller::setRumble(Joystick::RumbleType side, float intensity)
	{
		joystick->SetRumble(side, intensity);
	}
}
