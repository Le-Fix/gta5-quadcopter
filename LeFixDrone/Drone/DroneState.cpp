#include "DroneState.h"

DroneState::DroneState()
{

}

DroneState::DroneState(Entity e, Quaternionf rot)
	: collider(e), rotCollider(rot)
{
	
}

DroneState::~DroneState()
{
}

void DroneState::set(Quaternionf rotation, Vector3f position, Vector3f velocity)
{
	deltaTime = 1.0f; //Not zero
	drot = Quaternionf(1.0f, 0.0f, 0.0f, 0.0f);
	rot = rotation;
	pos = position;
	vel = velocity;
}

void DroneState::apply()
{
	ENTITY_X::SET_ENTITY_COORDS(collider, pos);
	ENTITY_X::SET_ENTITY_QUATERNION(collider, rot * rotCollider);
	ENTITY_X::SET_ENTITY_VELOCITY(collider, vel);
}

void DroneState::read()
{
	//Temporary variable
	Quaternionf rotTemp = ENTITY_X::GET_ENTITY_QUATERNION(collider) * rotCollider.conjugate();

	//Implicitly determines rotation velocity
	drot = rot.conjugate() * rotTemp;
	deltaTime = GAMEPLAY::GET_FRAME_TIME();

	pos = ENTITY_X::GET_ENTITY_COORDS(collider);
	rot = rotTemp;
	vel = ENTITY_X::GET_ENTITY_VELOCITY(collider);
}