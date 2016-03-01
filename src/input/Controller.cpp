#include "Controller.h"

namespace ADBLib
{
	ctrlCfg::ctrlCfg()
	{
		id = 0;
		type = JOYSTICK;
		inverse = false;
		btn.toggle = false;
		btn.cooldown = 0.250; //Seconds
		btn.cooldownTimer = new Timer;
		jys.maxVal = 1.0;
		jys.minVal = -1.0;
		jys.deadzone = 0.1;
		jys.equ.parse("x"); //Start off with no alterations to the joystick equation
	}

	Controller::~Controller()
	{
		delete joystick;
		for (auto profile = profiles.begin(); profile != profiles.end(); profile++) //Delete cooldown timers
		{
			for (auto control = profile->second.begin(); control != profile->second.end(); control++)
				delete control->second.btn.cooldownTimer;
		}
	}

	/**
	 * @brief Gets the raw button setting as true or false, disregarding any active cooldowns.
	 * @param ID The ID of the button. ID changes depending on controller mode (X-Mode or D-Mode)
	 */
	bool Controller::getButtonRaw(int ID)
	{
		if ((ID >= 15 || ID <= 0) && joystick != nullptr)
			return false; //Out-of-bounds IDs not accepted.
		return joystick->GetRawButton(ID);
	}

	/**
	 * @brief Gets the raw axis value.
	 * @param ID The ID of the joystick. ID changes depending on controller mode (X-Mode or D-Mode)
	 */
	double Controller::getJoystickRaw(int ID)
	{
		if (ID >= 0 && ID <= 15 && joystick != nullptr)
			return joystick->GetRawAxis(ID);
		else
			return 0;
	}

	/**
	 * @brief Sets the joystick to get data from.
	 * @param newJoystick A shared pointer to the joystick to use.
	 */
	void Controller::setJoystick(Joystick* newJoystick)
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
		if (joystick != nullptr)
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
	 *		<control type="joystick" name="intakeWheels" id="5">
	 *		  <maxInput value="1.0"/>
	 *		  <minInput value="-1.0"/>
	 *	  	  <deadzone value="0.1"/>
	 *		  <equation value="x"/>
	 *		</control>
	 *	  </profile>
	 *	 </ControlConfig>
	 *
	 * @param cfgFile The path/filename to the config file.
	 * @note RoboRIOs are UNIX-y! This means that your directory (even if it's just the root directory!) should start out with a '/' !
	 * @note Not setting a control parameter in the file will cause default values to be used! If you suspect error, check the log!
	 */
	void Controller::parseConfig(string filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename.c_str());
		Logger::log(string("XML Load Result: ") + result.description(), "sysLog");

		for (pugi::xml_node profile: doc.child("ControlConfig").children("profile"))
		{ //Loop through all profiles
			unordered_map<string, ctrlCfg> profileSet;
			Logger::log(string("Found profile ") + profile.attribute("name").as_string(), "sysLog");

			for (auto control = profile.child("control"); control; control = control.next_sibling())
			{ //Loop through all controls
				ctrlCfg newCtrl;
				newCtrl.id = control.attribute("id").as_int();
				Logger::log(string("Found control ") + control.attribute("name").as_string(), "sysLog");

				if (control.attribute("type").as_string() == string("button"))
				{
					Logger::log("Control is of type button", "sysLog");
					newCtrl.type = ctrlCfg::BUTTON;
					newCtrl.inverse = control.child("inverse").attribute("value").as_bool();
					newCtrl.btn.cooldown = control.child("cooldown").attribute("value").as_double();
					newCtrl.btn.cooldownTimer->Start();
					newCtrl.btn.toggle = control.child("toggle").attribute("value").as_bool();
					newCtrl.btn.on = new bool(false); //Can't do this in a constructor...
				}
				else
				{
					Logger::log("Control is of type joystick", "sysLog");
					newCtrl.type = ctrlCfg::JOYSTICK;
					newCtrl.jys.maxVal = control.child("maxInput").attribute("value").as_double();
					newCtrl.jys.minVal = control.child("minInput").attribute("value").as_double();
					newCtrl.jys.deadzone = control.child("deadzone").attribute("value").as_double();
					try
					{
						newCtrl.jys.equ.parse(control.child("equation").attribute("value").as_string());
					}
					catch (const parse_error &ex)
					{
						Logger::log(string("Failed to parse equation ' ") + control.child("equation").attribute("value").as_string() + string(": ") + ex.what(), string("sysLog"), error);
						Logger::log("Defaulting to equation y=x", "sysLog", error);
						newCtrl.jys.equ.parse("x");
					}
				}
				profileSet[control.attribute("name").as_string()] = newCtrl;
			}
			profiles[profile.attribute("name").as_string()] = profileSet;

			if (profile.attribute("active").as_bool()) //Automatically switch to default active profiles
			{
				Logger::log(string("Switching to active profile ") + profile.attribute("name").as_string(), "sysLog");
				switchProfile(profile.attribute("name").as_string());
			}

		}
		Logger::log("Finished parsing config file", "sysLog");
	}

	/**
	 * @brief Switches out the current control profile for another profile, specified by name.
	 * @param profileName The name of the profile to switch to as a string.
	 */
	void Controller::switchProfile(string profileName)
	{
		if (profiles.count(profileName) != 0)
			currentProfile = profileName;
		else
			Logger::log("Couldn't switch to profile - it doesn't exist!", "sysLog", error);
	}

	/**
	 * @brief Get the value of a set control, referenced by name
	 * @param name The name of the control. Referenced in the XML configuration file.
	 * @return If the control is a button, nonzero for true and zero for false (duh). Else, a double.
	 * @note If you get inexplicable crashes originating from this class, check to make sure you gave it a joystick.
	 */
	double Controller::operator[](const string& name)
	{
		if (profiles[currentProfile].count(name) == 0)
		{
			Logger::log("Could not find control " + name, "sysLog", error);
			return 0; //There is no control by this name!
		}
			
		ctrlCfg control = profiles[currentProfile][name];
		if (control.type == ctrlCfg::BUTTON)
		{
			bool ret = false;
			if (control.btn.toggle)
			{
				//Toggles
				if (joystick->GetRawButton(control.id) && control.btn.cooldownTimer->Get() >= control.btn.cooldown)
					*control.btn.on = !*control.btn.on;
				ret = *control.btn.on;
			}
			else
			{
				//Non-toggle, still needs cooldown
				if (joystick->GetRawButton(control.id) && control.btn.cooldownTimer->Get() >= control.btn.cooldown)
					ret = true;
			}

			if (control.btn.cooldownTimer->Get() >= control.btn.cooldown)
			{ //Reset cooldown timer if its time is too high.
				control.btn.cooldownTimer->Stop();
				control.btn.cooldownTimer->Reset(); //All of these function calls may not be needed, but screw timers.
				control.btn.cooldownTimer->Start();
			}
			return control.inverse ? !ret : ret;
		}
		else
		{ //It's a joystick
			double value = joystick->GetRawAxis(control.id);
			if (fabs(value) <= control.jys.deadzone)
				return 0;
			double slope = (control.jys.maxVal - control.jys.minVal) / 2.0;
			return (slope * value) - slope + control.jys.maxVal;

		}
		return 0; //STOP YELLING AT ME, ECLIPSE
	}
}
