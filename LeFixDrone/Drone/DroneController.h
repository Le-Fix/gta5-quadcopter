#pragma once

#include "..\LeFixDrone.h"
#include "..\Steuerung.h"
#include "DroneState.h"
#include "PID_Quat.h"
#include "PID_Vect.h"

class DroneController
{
public:
	virtual void reset(const DroneState &currentState) = 0;
	virtual void update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y) = 0;

	virtual Quaternionf getDesiredRot() = 0;
	virtual float getThrottle() = 0;
};