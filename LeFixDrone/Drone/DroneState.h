#pragma once

#include "..\LeFixDrone.h"

using namespace Eigen;

class DroneState
{
public:
	DroneState();
	DroneState(Entity e, Quaternionf rot);
	~DroneState();

	void set(Quaternionf rot, Vector3f pos, Vector3f vel);
	void apply();
	void read();

	Quaternionf rot, drot;	//Rotation and implicitly rotation velocity
	Vector3f pos, vel;		//Position and velocity
	float deltaTime;

private:
	Quaternionf rotCollider;
	Entity collider;
};

