#include "PID_Quat.h"



PID_Quat::PID_Quat()
	: PID()
{
	reset();
}


PID_Quat::~PID_Quat()
{
}

void PID_Quat::reset()
{
	errorP = Vector3f(0.0f, 0.0f, 0.0f);
	errorI = Vector3f(0.0f, 0.0f, 0.0f);
}

void PID_Quat::update(const Quaternionf &ist, const Quaternionf &soll, const float &dT)
{
	if (dT == 0.0f) return;

	AngleAxisf temp = AngleAxisf(ist.conjugate() * soll);	//Error Rotation As Angle Axis

	errorPOld = errorP;	//Needed for derivative

	errorP = temp.axis() * temp.angle();			//Proportional
	errorI = errorI + errorP;		//Integral
	Vector3f preErrorD = (errorP - errorPOld) / dT;	//Differential

	errorD = 0.2f * (errorD + 4.0f * preErrorD); //Lags
}

Vector3f PID_Quat::getOutput()
{
	//Output
	return errorP * kP + errorI * kI + errorD * kD;
}