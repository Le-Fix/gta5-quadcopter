#pragma once

#include "PID.h"

using namespace Eigen;

class PID_Quat :
	public PID
{
public:
	PID_Quat();
	~PID_Quat();

	void reset();
	void update(const Quaternionf &ist, const Quaternionf &soll, const float &dT);
	Vector3f getOutput();

private:
	Vector3f errorP, errorI, errorD, errorPOld;
};

