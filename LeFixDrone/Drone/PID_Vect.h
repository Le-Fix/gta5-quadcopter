#pragma once
#include "PID.h"

class PID_Vect :
	public PID
{

public:
	PID_Vect();
	~PID_Vect();

	void reset();
	void update(const Vector3f &ist, const Vector3f &soll, const float &dT);
	Vector3f getOutput();

private:
	Vector3f errorP, errorI, errorD, errorPOld;
};

