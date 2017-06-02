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
	PID_Vect posPID;			//internal position PID
	Vector3f endPos, slerpPos;	//Position logging and smooth pos
	float endHeading;			//Desired Heading

	Quaternionf calcRot;		//Output Rotation
	float calcThrottle;			//Output Throttle
};

