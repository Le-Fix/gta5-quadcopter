#pragma once
#include "DroneController.h"
class DroneControllerAcro :
	public DroneController
{
public:
	DroneControllerAcro();
	~DroneControllerAcro();

	void reset(const DroneState &currentState);
	void update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y);

	Quaternionf getDesiredRot();
	float getThrottle();

private:
	Quaternionf endRot;
	float contThrottle;
};

