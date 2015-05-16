#include "Path.h"

namespace ADBLib
{
	Path::Path(Drivebase* newDrivebase, RoboPositioner* newRPS)
	{
		drivebase = newDrivebase;
		rps = newRPS;
		active = false;
	}
}
