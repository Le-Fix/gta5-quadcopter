#pragma once

#include "..\LeFixDrone.h"

class PID
{
public:
	PID();
	~PID();

	virtual void reset() = 0;

	float kP, kI, kD;	//PID params
};

