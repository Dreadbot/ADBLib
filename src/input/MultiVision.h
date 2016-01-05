#pragma once

#include <WPILib.h>
#include <unordered_map>
#include "Camera.h"
using std::unordered_map;

namespace ADBLib
{
	/**
	 * @brief Camera manager-type class that allows easy camera switching/handling.
	 */
	class MultiVision
	{
	public:
		MultiVision();
		bool switchCamera(string camName);
		void postImage();
		Image* getCurrentImage();
		Camera* getCamera(string camName);
	protected:
		unordered_map<string, Camera*> cameras;
		Camera* current;
	};
}
