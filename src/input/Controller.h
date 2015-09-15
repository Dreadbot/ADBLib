#include <WPILib.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "../misc/Equation.h"
using std::unordered_map;
using std::vector;
using std::string;

namespace ADBLib
{
	/**
	* @brief A wrapper class for the WPILib Joystick class, adding some stuff to take the headache out of working with raw inputs
	*/
	class Controller
	{
	public:
		Controller();
		Controller(std::shared_ptr<Joystick> newJoystick);
		bool getButtonRaw(int ID);
		double getJoystickRaw(int ID);
		void setJoystick(std::shared_ptr<Joystick> newJoystick);
		void setRumble(Joystick::RumbleType side, float intensity = 1.f);
		void parseConfig(string filename);
		void switchProfile(string profileName);
		double operator[](const string& name);
	protected:
		class ctrlCfg
		{
		public:
			ctrlCfg();
			enum {BUTTON, JOYSTICK} type;	//!< The type of control this is.
			unsigned int id;				//!< Control ID, needed regardless of button vs joystick
			bool inverse;					//!< Inverse - if button, inverts. If joystick, flips the axis... sort of
			struct btnCfg
			{
				bool toggle;
				double cooldown;
			} btn;
			struct jysCfg
			{
				double maxVal;				//!< Minimum value for a joystick. Allows control scaling.
				double minVal;				//!< Maximum value for a joystick. Allows control scaling.
				double deadzone;			//!< Deadzone to use
				Equation equ;
			} jys;
		};

		unordered_map<string, unordered_map<string, ctrlCfg>> profiles;
		string currentProfile;
		std::shared_ptr<Joystick> joystick;
		Timer cooldownTimers[15];	// For buttons only
	};
}
