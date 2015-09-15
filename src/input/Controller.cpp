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

	/**
	 * @brief Loads control settings from a config file, because hard-coded controls are awful.
	 *
	 *	Config files should be structured as follows:
	 *	* The root node should be called "ControlConfig".
	 *	* The children underneath the root node should be called "profile" with an attribute "name".
	 *	  This name is the name for a specific control profile. There should also be an attribute "active",
	 *	  which should be either true or false. This sets which control profile is active.
	 *	* Nodes underneath a profile node should be named "control". They should have an attribute "type"
	 *	  (either button or joystick), an attribute "name", which is how you will refer to the control in
	 *	  the code, and an "id" attribute, for the control ID.
	 *	* A control node's children should vary depending on control type.
	 *	  	* All children will name the type of parameter, whereas an XML attribute ("value") specifies the
	 *	  	  value.
	 *	  	* Button controls need only have "toggle" (boolean) and "cooldown" (double) parameters..
	 *	  	* Joystick controls have four parameters: maxInput, minInput, deadzone, and equation (how the resulting value is modified; negative-blind).
	 *	  	* Both button and joystick controls can accept an "inverse" (boolean) parameter, to invert output.
	 *
	 * An example (tiny) XML config file follows (you might need to check the source code, doxygen screws up the tabs!):
	 *
	 * <ControlConfig>
	 *	  <profile name="Sourec" active="false">
	 *		<control type="button" id="0" name="raiseForks">
	 *		  <toggle value="false"/>
	 *		  <cooldown value="0.250"/>
	 *		</control>
	 *		<control type="joystick" name="intakeWheels">
	 *		  <maxInput value="1.0"/>
	 *		  <minInput value="-1.0"/>
	 *	  	  <deadzone value="0.1"/>
	 *		  <equation value="x"/>
	 *		</control>
	 *	  </profile>
	 *	 </ControlConfig>
	 *
	 * @param cfgFile The path/filename to the config file.
	 * @note RoboRIOs are LINUX BASED! This means that your directory (even if it's just the root directory!) should start out with a '/' !
	 * @note NOT SETTING A CONTROL PARAMETER IN THE FILE WILL CAUSE DEFAULT VALUES TO BE USED! IF YOU SUSPECT ERROR, CHECK THE LOG!
	 */
	void Controller::parseConfig(string filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(filename.c_str());
		//Logger::getInstance()->log("sysLog", result.status, hydsys);

		for (auto profile = doc.child("ControlConfig").child("profile"); profile; profile = profile.next_sibling())
		{ //Loop through all profiles
			unordered_map<string, ctrlCfg> profileSet;

			for (auto control = profile.child("control"); control; control = control.next_sibling())
			{ //Loop through all controls
				ctrlCfg newCtrl;
				newCtrl.id = control.attribute("id").as_int();
				if (control.attribute("type").as_string() == string("button"))
				{
					newCtrl.type = ctrlCfg::BUTTON;
					newCtrl.inverse = control.child("inverse").attribute("value").as_bool();
					newCtrl.btn.cooldown = control.child("cooldown").attribute("cooldown").as_double();
					newCtrl.btn.toggle = control.child("toggle").attribute("value").as_bool();
				}
				else
				{
					newCtrl.type = ctrlCfg::JOYSTICK;
					newCtrl.jys.maxVal = control.child("maxInput").attribute("value").as_double();
					newCtrl.jys.minVal = control.child("minInput").attribute("value").as_double();
					newCtrl.jys.deadzone = control.child("deadzone").attribute("value").as_double();
					newCtrl.jys.equ.parse(control.child("equation").attribute("value").as_string());
				}
				profileSet[control.attribute("name").as_string()] = newCtrl;
			}
			profiles[profile.attribute("name").as_string()] = profileSet;
		}
	}

	/**
	 * @brief Sets default control values.
	 * No inverse, id = 0, standard joystick ranges, no toggle, a cooldown of 250 ms, and a deadzone of 0.1
	 */
	Controller::ctrlCfg::ctrlCfg()
	{
		id = 0;
		type = JOYSTICK;
		inverse = false;
		btn.toggle = false;
		btn.cooldown = 0.250; //Seconds
		jys.maxVal = 1.0;
		jys.minVal = -1.0;
		jys.deadzone = 0.1;
		jys.equ.parse("x"); //Start off with no alterations to the joystick equation
	}
}
