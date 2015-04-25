#include "Camera.h"

namespace ADBLib
{
	/**
	 * \brief Creates a new camera with a name ID'd by the roboRIO.
	 * \param newName The name of the camera, found through the roboRIO.
	 */
	Camera::Camera(string newName)
	{
		name = newName;
		enabled = false;
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	}
	bool Camera::start()
	{
		if (getEnabled())
			return true; //This camera is already enabled.

		IMAQdxError imaqError = IMAQdxOpenCamera(name.c_str(), IMAQdxCameraControlModeController, &camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::getInstance()->log(name + " IMAQdxOpenCamera error - " + std::to_string((long)imaqError), Hydra::error);
			return false;
		}

		imaqError = IMAQdxConfigureGrab(camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::getInstance()->log(name + " IMAQdxConfigureGrab error - " + std::to_string((long)imaqError), Hydra::error);
			return false;
		}
		IMAQdxStartAcquisition(camSession);
		enabled = true;
		return true;
	}
	bool Camera::stop()
	{
		IMAQdxStopAcquisition(camSession);
		IMAQdxError imaqError = IMAQdxCloseCamera(camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::getInstance()->log(name + " IMAQdxCloseCamera error - " + std::to_string((long)imaqError), Hydra::error);
			return false;
		}
		enabled = false;
		return true;
	}
	bool Camera::getEnabled()
	{
		return enabled;
	}
	Image* Camera::getImage()
	{
		IMAQdxGrab(camSession, frame, true, nullptr);
		return frame;
	}
}
