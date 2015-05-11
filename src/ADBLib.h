#pragma once
#include <WPILib.h>

//Drivebase
#include "drivebase/Drivebase.h"
#include "drivebase/MecanumDrive.h"
#include "drivebase/OmniDrive.h"
#include "drivebase/TractionDrive.h"
#include "drivebase/OctocanumDrive.h"

//Input
#include "input/Controller.h"
#include "input/MultiVision.h"
#include "input/Camera.h"

//Output
#include "output/SimpleMotor.h"
#include "output/SimplePneumatic.h"
#include "output/MotorGroup.h"
#include "output/PneumaticGroup.h"
#include "output/PIDMotor.h"

//External library files
#include "../lib/hydra/Logger.h"
#include "../lib/hydra/Matrix.h"
#include "../lib/hydra/Vector3D.h"
