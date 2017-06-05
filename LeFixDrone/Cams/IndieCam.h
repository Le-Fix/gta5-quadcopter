#pragma once

#include "../LeFixDrone.h"

class IndieCam
{
public:
	IndieCam();
	virtual ~IndieCam();

	void setActive();
	virtual void update();

	Cam cam;
};

