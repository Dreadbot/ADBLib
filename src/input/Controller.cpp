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
	}

	ctrlCfg::btnCfg::~btnCfg()
	{
		delete cooldownTimer;
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
		Hydra::Logger::log(result.description(), "sysLog", Hydra::hydsys);

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
				//profileSet[control.attribute("name").as_string()] = newCtrl;
			}
			profiles[profile.attribute("name").as_string()] = profileSet;

			if (profile.attribute("active").as_bool()) //Automatically switch to default active profiles
				switchProfile(profile.attribute("name").as_string());
		}
	}

	/**
	 * @brief Switches out the current control profile for another profile, specified by name.
	 * @param profileName The name of the profile to switch to as a string.
	 */
	void Controller::switchProfile(string profileName)
	{
		if (profiles.count(profileName) != 0)
			currentProfile = profileName;
	}

	/**
	 * @brief Get the value of a set control, referenced by name
	 * @param name The name of the control. Referenced in the XML configuration file.
	 * @return If the control is a button, nonzero for true and zero for false (duh). Else, a double.
	 */
	double Controller::operator[](const string& name)
	{
		ctrlCfg control;// = profiles[currentProfile][name]; //NOT a 2D array!
		if (control.type == ctrlCfg::BUTTON)
		{
			if (!control.btn.toggle)
				return control.inverse ? !joystick->GetRawButton(control.id) : joystick->GetRawButton(control.id);
			else if (joystick->GetRawButton(control.id) || (!joystick->GetRawButton(control.id) && control.inverse))
			{
				if (control.btn.cooldownTimer->Get() >= control.btn.cooldown)
				{ //The cooldown expired
					control.btn.cooldownTimer->Reset();
					control.btn.cooldownTimer->Start();
					return !control.inverse;
				}
				return control.inverse;
			}
		}
		else
		{ //It's a joystick
			double value = joystick->GetRawAxis(control.id);
			double temp = value;
			value = control.jys.equ.evaluate(fabs(value));
			value = std::copysign(temp, value); //Copysign from temp to value?

			//scale values... should work
			temp = value;
			value = fabs(value) * ((control.jys.maxVal - control.jys.minVal) * 0.5f);
			return std::copysign(temp, value) + control.jys.minVal; //TODO: Verify

		}
		return 0; //STOP YELLING AT ME, ECLIPSE
	}
}
