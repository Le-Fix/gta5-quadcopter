#pragma once

#include "IndieCam.h"

class FollowPointCam :
	public IndieCam
{
public:
	FollowPointCam(float d);
	~FollowPointCam();

	void update(const Vector3f& point);
	void setDistance(float d);

	Vector3f getVel();
	Vector3f getPos();

private:
	Vector3f endPos, slerpPos, vel;
	float distance;
};

