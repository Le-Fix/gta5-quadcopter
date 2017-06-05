#include "FollowPointCam.h"

FollowPointCam::FollowPointCam()
	: IndieCam(), distance(1.0f)
{
}

FollowPointCam::FollowPointCam(float d)
	: IndieCam(), distance(d)
{
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
	if ((point - pos).squaredNorm() > distance*distance)
	{
		float dT = GAMEPLAY::GET_FRAME_TIME();
		Vector3f posNew = point + distance*(point - pos).normalized();
		pos += (posNew - pos) * dT;

		CAM_X::SET_CAM_COORD(cam, pos);
	}
	CAM_X::POINT_CAM_AT_COORD(cam, point);
}
