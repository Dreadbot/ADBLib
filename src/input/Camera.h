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
		bool start();			//!< Starts this camera.
		bool stop();			//!< Stops this camera.
		bool getEnabled();		//!< Gets the status of the camera - true for on, false for off.
		Image* getImage();		//!< Gets the image on this camera.
	protected:
		string name;
		Image* frame;
		bool enabled;
		IMAQdxSession camSession;
	};
}
