#include "DroneControllerLevel.h"



DroneControllerLevel::DroneControllerLevel()
	: endPos(0.0f, 0.0f, 100.0f), endHeading(0.0f), posPID()
{
	posPID.kP = 0.1f;
	posPID.kI = 0.001f;
	posPID.kD = 0.1f;
}

DroneControllerLevel::~DroneControllerLevel()
{
}

void DroneControllerLevel::reset(const DroneState &currentState)
{
	endHeading = DegToRad(getHeading(currentState.rot._transformVector(Vector3f(0.0f, 1.0f, 0.0f))));
	endPos = currentState.pos;
	slerpPos = endPos;
	posPID.reset();
}

void DroneControllerLevel::update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y)
{
	static Vector3f up = Vector3f(0.0f, 0.0f, 1.0f);

	float inputYaw = -stick_left_x;
	float inputZ = stick_left_y;	//Pos. Up, Neg. Down
	float inputY = stick_right_y;	//Pos. Forward, Neg. Backward
	float inputX = stick_right_x;	//Pos. Right, Neg. Left

	float dT = GAMEPLAY::GET_FRAME_TIME();

	//Update end state
	endHeading += DegToRad(180.0f) * inputYaw * dT;

	Vector3f relDeltaPos = Vector3f(inputX, inputY, inputZ)*dT*Settings::droneMaxVel.get()*0.8f;

	Quaternionf idleRot = Quaternionf(AngleAxisf(endHeading, up));
	endPos += idleRot._transformVector(relDeltaPos);

	slerpPos = (1.0f - dT)*slerpPos + dT*endPos;

	//Update PID
	posPID.update(currentState.pos, slerpPos, dT);
	Vector3f output = posPID.getOutput();

	//Infinite possibilities to achieve this acceleration axis
	Quaternionf closeIdle = idleRot *			Quaternionf::FromTwoVectors(up,          idleRot.conjugate()._transformVector(output)); //Closest quat to idle
	Quaternionf closeCurr = currentState.rot *	Quaternionf::FromTwoVectors(up, currentState.rot.conjugate()._transformVector(output)); //Closest quat to current

	calcRot = closeCurr.slerp(dT*10.0f, closeIdle); //Slowly go to closest idle quat
	
	//Calculated throttle
	calcThrottle = 0.5f*(calcThrottle + output.dot(currentState.rot._transformVector(Vector3f(0.0, 0.0, 1.0f))));

	//Negative throttle
	if (!Settings::drone3DFly.get() && calcThrottle < 0.0f) calcThrottle = 0.0f;
}

Quaternionf DroneControllerLevel::getDesiredRot()
{
	return calcRot;
}

float DroneControllerLevel::getThrottle()
{
	return calcThrottle;
}
