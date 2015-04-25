#include <WPILib.h>
#include <memory>

namespace ADBLib
{
	class Controller
	{
	public:
		Controller();
		Controller(std::shared_ptr<Joystick> newJoystick);
		bool getButton(int ID, float cooldown = 0.f);
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
	};
}
