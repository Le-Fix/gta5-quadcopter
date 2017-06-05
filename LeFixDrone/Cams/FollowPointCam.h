#pragma once

#include "IndieCam.h"

class FollowPointCam :
	public IndieCam
{
public:
	FollowPointCam();
	FollowPointCam(float d);
	~FollowPointCam();

	void update(const Vector3f& point);
	void setDistance(float d);

private:
	Vector3f pos;
	float distance;
};

