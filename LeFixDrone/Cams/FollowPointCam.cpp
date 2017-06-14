#include "FollowPointCam.h"

FollowPointCam::FollowPointCam(float d)
	: distance(d)
{
	endPos = Vector3f(0.0f, 0.0f, 0.0f);
	slerpPos = Vector3f(0.0f, 0.0f, 0.0f);
}

FollowPointCam::~FollowPointCam()
{
}

void FollowPointCam::setDistance(float d)
{
	distance = d;
}

void FollowPointCam::update(const Vector3f& point)
{
	float dT = GAMEPLAY::GET_FRAME_TIME();

	endPos = (endPos - point).normalized()*distance + point;

	vel = (endPos - slerpPos)*5.0f;

	slerpPos += vel*dT;

	CAM_X::SET_CAM_COORD(cam, slerpPos);
	CAM_X::POINT_CAM_AT_COORD(cam, point);
}

Vector3f FollowPointCam::getVel()
{
	return vel;
}

Vector3f FollowPointCam::getPos()
{
	return slerpPos;
}