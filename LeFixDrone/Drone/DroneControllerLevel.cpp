#include "DroneControllerLevel.h"

#include "../Settings.h"
#include "../LeFixEnums.h"

DroneControllerLevel::DroneControllerLevel()
	: _endPos(0.0f, 0.0f, 100.0f), _endRot(0.0f, 0.0f, 0.0f, 1.0f), posPID()
{
	posPID.kP = 0.3f;
	posPID.kI = 0.001f;
	posPID.kD = 0.2f;
}

DroneControllerLevel::~DroneControllerLevel()
{
}

void DroneControllerLevel::reset(const DroneState &currentState)
{
	float currentHeading = DegToRad(getHeading(currentState.rot._transformVector(Vector3f(0.0f, 1.0f, 0.0f))));
	_endRot = Quaternionf(AngleAxisf(currentHeading, up));
	_endPos = currentState.pos;
	posPID.reset();
}

void DroneControllerLevel::update(const DroneState &currentState, const float &stick_left_x, const float &stick_left_y, const float &stick_right_x, const float &stick_right_y)
{
	float inputYaw = -stick_left_x;
	float inputZ = stick_left_y;	//Pos. Up, Neg. Down
	float inputY = stick_right_y;	//Pos. Forward, Neg. Backward
	float inputX = stick_right_x;	//Pos. Right, Neg. Left

	float dT = GAMEPLAY::GET_FRAME_TIME();

	//update End State
	updateEndState(inputX, inputY, inputZ, inputYaw);

	//Update PID
	posPID.update(currentState.pos, _endPos, dT);
	Vector3f output = posPID.getOutput();

	float norm = output.norm();
	output /= norm; //normalize

	//Infinite possibilities to achieve this acceleration axis
	Quaternionf closeEnd  = _endRot *			Quaternionf::FromTwoVectors(up,          _endRot.conjugate()._transformVector(output)); //Closest quat to end
	Quaternionf closeCurr = currentState.rot *	Quaternionf::FromTwoVectors(up, currentState.rot.conjugate()._transformVector(output)); //Closest quat to current

	_calcRot = closeCurr.slerp(0.4f, closeEnd); //Slowly go to closest idle quat
	
	//Current Drone Roation and desired Force direction
	float alignment = output.dot(currentState.rot._transformVector(Vector3f(0.0, 0.0, 1.0f)));
	
	//Throttle cap
	if (norm > 1.0f) norm = 1.0f;

	_calcThrottle = norm * alignment;

	//Negative throttle
	if (!Settings::drone3DFly && _calcThrottle < 0.0f) _calcThrottle = 0.0f;
}

void DroneControllerLevel::updateEndState(const float &inputX, const float &inputY, const float &inputZ, const float &inputYaw)
{
	float dT = GAMEPLAY::GET_FRAME_TIME();

	//Relative Position Delta
	Vector3f relDeltaPos = Vector3f(inputX, inputY, inputZ)*dT*Settings::droneMaxVel*0.8f;

	if (Settings::camMode == LeFix::camModeC1 || Settings::camMode == LeFix::camModeDF)
	{
		//Idle Quaternion
		Camera currentCam = CAM::GET_RENDERING_CAM();
		float remoteHeading =getHeading(CAM_X::GET_CAM_QUATERNION(currentCam)._transformVector(Vector3f(0.0f, 1.0f, 0.0f)));
		_endRot = Quaternionf(AngleAxisf(DegToRad(remoteHeading), up));
	}
	else
	{
		//Idle Quaternion
		_endRot *= Quaternionf(AngleAxisf(DegToRad(180.0f) * inputYaw * dT, up));

	}

	//Transform Position Delta by Idle Quaternion
	_endPos += _endRot._transformVector(relDeltaPos);
}

Quaternionf DroneControllerLevel::getDesiredRot()
{
	return _calcRot;
}

float DroneControllerLevel::getThrottle()
{
	return _calcThrottle;
}
