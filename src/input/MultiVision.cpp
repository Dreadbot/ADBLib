#include "MultiVision.h"

namespace ADBLib
{
	/**
	 * @brief Constructor; sets the current enabled camera to nullptr.
	 */
	MultiVision::MultiVision()
	{
		current = nullptr;
	}

	/**
	 * @brief Switches to the new camera, automatically stopping the previous camera and starting the new one.
	 * @param camName The name of the camera. This can be found through the roboRIO web page.
	 * @return Status of the new camera; true for enabled, false otherwise.
	 * @note If the camera you specified does not exist, a new camera will be created using the name you specified.
	 */
	bool MultiVision::switchCamera(string camName)
	{
		if (cameras.count(camName) == 0)
			cameras[camName] = new Camera(camName);
		if (current != nullptr)
			current->stop();

		current = cameras[camName];
		return current->start();
	}

	/**
	 * @brief Post an image from the current camera to the Camera Server (i.e. to the dashboard)
	 */
	void MultiVision::postImage()
	{
		if (current == nullptr)
			return;
		if (!current->getEnabled()) //This check MUST be done after the previous nullptr check - otherwise if there was a nullptr error, this getEnabled function would still be called.
			return;

		CameraServer::GetInstance()->SetImage(current->getImage());
	}

	/**
	 * @brief Gets an image from the current camera.
	 * @return An IMAQ_IMAGE_RGB image object thingy.
	 */
	Image* MultiVision::getCurrentImage()
	{
		if (current == nullptr)
			return nullptr;
		if (!current->getEnabled()) //This check MUST be done after the previous nullptr check - otherwise if there was a nullptr error, this getEnabled function would still be called.
			return nullptr;

		return current->getImage();
	}

	/**
	 * @brief Gets a camera with the given name.
	 * @param camName The name of the camera. Found in the roboRIO.
	 * @return A camera object. Should never be nullptr.
	 * @note If a camera under the given name is not found, creates a new one under the given name!
	 */
	Camera* MultiVision::getCamera(string camName)
	{
		if (cameras.count(camName) == 0)
			cameras[camName] = new Camera(camName);
		return cameras[camName];
	}
}
