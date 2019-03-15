#pragma once

#include <WPILib.h>
#include <string>
#include "../misc/Logger.h"
using std::string;

namespace ADBLib
{
	class Camera
	{
		public:
			Camera(string newName);
			bool start();
			bool stop();
			bool getEnabled();
			Image* getImage();
		protected:
			string name;
			Image* frame;
			bool enabled;
			IMAQdxSession camSession;
	};
}
