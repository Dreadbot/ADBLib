#pragma once

#include <WPILib.h>
#include <string>
#include "../../lib/hydra/Logger.h"
using std::string;
using Hydra::Logger;

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
