#include "PID_Vect.h"



PID_Vect::PID_Vect()
	: PID()
{
	reset();
}


PID_Vect::~PID_Vect()
{
}

void PID_Vect::reset()
{
	errorP = Vector3f(0.0f, 0.0f, 0.0f);
	errorI = Vector3f(0.0f, 0.0f, 0.0f);
}

void PID_Vect::update(const Vector3f &ist, const Vector3f &soll, const float &dT)
{
	if (dT == 0.0f) return;

	errorPOld = errorP;	//Needed for derivative

	errorP = soll - ist;							//Proportional
	errorI = errorI + errorP;						//Integral
	Vector3f preErrorD = (errorP - errorPOld) / dT;	//Differential

	errorD = 0.2f * (errorD + 4.0f * preErrorD); //Lags
}

Vector3f PID_Vect::getOutput()
{
	//Output
	return errorP * kP + errorI * kI + errorD * kD;
}