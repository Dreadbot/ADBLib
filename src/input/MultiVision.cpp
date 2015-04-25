#include "MultiVision.h"

namespace ADBLib
{
	Camera* MultiVision::current = nullptr;
	bool MultiVision::switchCamera(string camName)
	{
		if (cameras.count(camName) == 0 || current == nullptr)
			return false;

		current->stop();
		current = &cameras[camName].second;
		return current->start();
	}
	void MultiVision::postImage()
	{
		if (current == nullptr)
			return;
		if (!current->getEnabled()) //This check MUST be done after the previous nullptr check - otherwise if there was a nullptr error, this getEnabled function would still be called.
			return;

		CameraServer::GetInstance()->SetImage(current->getImage());
	}
	Image* MultiVision::getCurrentImage()
	{
		if (current == nullptr)
			return nullptr;
		if (!current->getEnabled()) //This check MUST be done after the previous nullptr check - otherwise if there was a nullptr error, this getEnabled function would still be called.
			return nullptr;

		return current->getImage();
	}
	Camera* MultiVision::getCamera(string camName)
	{
		if (cameras.count(camName) == 0)
			return nullptr;
		return &cameras[camName].second;
	}
}
