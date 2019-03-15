#include "Camera.h"

namespace ADBLib
{
	/**
	 * @brief Creates a new camera with a name ID'd by the roboRIO.
	 * @param newName The name of the camera, found through the roboRIO.
	 */
	Camera::Camera(string newName)
	{
		name = newName;
		enabled = false;
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	}

	/**
	 * @brief Starts the camera.
	 * @return True if successful, false otherwise.
	 * @note This function MUST be called before the camera can be used!
	 */
	bool Camera::start()
	{
		if (getEnabled())
			return true; //This camera is already enabled.

		IMAQdxError imaqError = IMAQdxOpenCamera(name.c_str(), IMAQdxCameraControlModeController, &camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::log(name + " IMAQdxOpenCamera error - " + std::to_string((long)imaqError), "sysLog", error);
			return false;
		}

		imaqError = IMAQdxConfigureGrab(camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::log(name + " IMAQdxConfigureGrab error - " + std::to_string((long)imaqError), "sysLog", error);
			return false;
		}

		IMAQdxStartAcquisition(camSession);
		enabled = true;
		return true;
	}

	/**
	 * @brief Stops the camera.
	 * @return True if successful, false otherwise.
	 * @note Camera must be started again before it can be used after this function is called!
	 */
	bool Camera::stop()
	{
		if (!enabled)
			return false; //Don't stop a stopped camera.

		IMAQdxStopAcquisition(camSession);
		IMAQdxError imaqError = IMAQdxCloseCamera(camSession);
		if (imaqError != IMAQdxErrorSuccess)
		{
			Logger::log(name + " IMAQdxCloseCamera error - " + std::to_string((long)imaqError), "sysLog", error);
			return false;
		}
		enabled = false;
		return true;
	}

	/**
	 * @brief Gets the status of the camera.
	 * @return True if enabled, false otherwise.
	 */
	bool Camera::getEnabled()
	{
		return enabled;
	}

	/**
	 * @brief Gets an image from the camera.
	 * @return An IMAQ_IMAGE_RGB image object thingy.
	 * @note Camera MUST be enabled for this to work!
	 */
	Image* Camera::getImage()
	{
		IMAQdxGrab(camSession, frame, true, nullptr);
		return frame;
	}
}
