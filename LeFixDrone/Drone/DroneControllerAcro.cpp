#include "DroneControllerAcro.h"


DroneControllerAcro::DroneControllerAcro()
	: endRot(1.0f, 0.0f, 0.0f, 0.0f)
{}

DroneControllerAcro::~DroneControllerAcro()
{
}

void DroneControllerAcro::reset(const DroneState &currentState)
{
	endRot = currentState.rot;
}

void DroneControllerAcro::update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y)
{
	//Input from Gamepad
	float inputPitch    = -stick_right_y;
	float inputRoll     =  stick_right_x;
	float inputYaw      = -stick_left_x;
	float inputThrottle =  stick_left_y;

	//INPUT TO CONTROL
	if (Settings::droneNoThrustDown)
		contThrottle = (inputThrottle + 1.0f) / 2.0f; //Linear, No throttle at -1.0 input value
	else
		contThrottle = inputThrottle; //Linear

	 //Negative throttle
	if (!Settings::drone3DFly && contThrottle < 0.0f) contThrottle = 0.0f;

	float contYaw   = Steuerung::calculateDegSecYaw(inputYaw);
	float contRoll  = Steuerung::calculateDegSecRoll(inputRoll);
	float contPitch = Steuerung::calculateDegSecPitch(inputPitch);

	//Input Delta Rotation as Angle Axis
	Vector3f axisRot = Vector3f(contPitch, contRoll, contYaw) * GAMEPLAY::GET_FRAME_TIME();
	float norm = axisRot.norm();	//Get Norm

	Quaternionf drot;
	if (norm == 0.0f) {
		//No rotation
		drot = Quaternionf(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		//Some rotation
		axisRot /= norm;		//Norm
		norm = DegToRad(norm);	//Degree to Radian
		drot = Quaternionf(AngleAxisf(norm, axisRot)); //Input Delta Rotation
	}
	endRot *= drot;
}

Quaternionf DroneControllerAcro::getDesiredRot()
{
	return endRot;
}

float DroneControllerAcro::getThrottle()
{
	return contThrottle;
}