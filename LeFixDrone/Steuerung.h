#pragma once

#include "Settings.h"

#include <math.h>

namespace Steuerung
{
	float calculateDegSecPitch(const float& inputP);
	float calculateDegSecRoll(const float& inputR);
	float calculateDegSecYaw(const float& inputY);
	float calculateDegSec(const float& input, const float& rcRate, const float& gRate, const float& expo);
}

