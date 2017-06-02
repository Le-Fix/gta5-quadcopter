#pragma once

#define NUM_PROP 4
#define NUM_RPM 2
#define _USE_MATH_DEFINES

#include "SHV\types.h"
#include "SHV\natives.h"
#include "SHV\enums.h"

#include <Eigen/Dense>
#include <string>
#include <cmath>

#include "Settings.h"
#include "Utils\UIutils.hpp"
#include "nativesExtended.h"


namespace LeFixDrone
{
	enum eRPM { mid, hgh };
	enum ePROP { FR, BR, BL, FL };
	enum eEXIT { exitNo, exitStart, exitHere };
	enum eCAMMODE { camModeD1, camModeD3, camModeC1, camModeC3 };
} // LeFixDrone
