#pragma once
#include "DroneController.h"
class DroneControllerLevel :
	public DroneController
{
public:
	DroneControllerLevel();
	~DroneControllerLevel();

	void reset(const DroneState &currentState);
	void update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y);

	Quaternionf getDesiredRot();
	float getThrottle();

private:

	void updateEndState(const float &inputX, const float &inputY, const float &inputZ, const float &inputYaw);

	PID_Vect posPID;			//internal position PID
	Vector3f _endPos;			//Position logging
	Quaternionf _endRot;		//Desired Heading

	Quaternionf _calcRot;		//Output Rotation
	float _calcThrottle;		//Output Throttle

	const Vector3f up = Vector3f(0.0f, 0.0f, 1.0f);
};

