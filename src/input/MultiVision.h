#pragma once

#include <WPILib.h>
#include <unordered_map>
#include "Camera.h"
using std::unordered_map;

namespace ADBLib
{
	class MultiVision
	{
	public:
		static bool switchCamera(string camName);	//!< Switch camera to a camera of the desired name. If no camera under this name exists, creates a new camera.
		static void postImage();					//!< Post an image from the current camera to the dashboard.
		static Image* getCurrentImage();			//!< Get an image from the currently active camera.
		static Camera* getCamera(string camName);	//!< Returns a pointer to a camera object. If no camera at this name exists, creates a new camera.
	protected:
		static unordered_map<string, Camera*> cameras;
		static Camera* current;
	};
}
