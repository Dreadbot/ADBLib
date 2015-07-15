#include <WPILib.h>
#include <memory>

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
		bool getButton(int ID, float cooldown = 0.f);
		bool getButtonToggle(int ID, float cooldown = 0.f);
		bool getButtonRaw(int ID);
		double getAxis(int ID);
		void setInputRange(int axisID, float max, float min);
		void setJoystick(std::shared_ptr<Joystick> newJoystick);
		void setRumble(Joystick::RumbleType side, float intensity = 1.f);
	protected:
		std::shared_ptr<Joystick> joystick;
		double axisRanges[15][2];	// [x][0] is lower, [x][1] is upper
		float cooldownTimes[15]; 	// For buttons only
		Timer cooldownTimers[15];	// For buttons only
		bool toggles[15];					// For buttons only
	};
}
