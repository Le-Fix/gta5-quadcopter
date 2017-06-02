#pragma once

#include "..\LeFixDrone.h"
#include "..\Input\Gamepad.h"
#include "..\AudioHandler.h"

#include "DroneState.h"
#include "DroneControllerAcro.h"
#include "DroneControllerLevel.h"
#include "PID_Quat.h"

using namespace Eigen;

class Drone
{
public:
	static void refreshSettingsStatic();
	//Member
	void refreshSettingsDynamic();
	void refreshCamMode();

	Drone(Vector3f pos, Vector3f vel, Quaternionf rot);
	~Drone();

	void update(Gamepad &gamepad);

	//Getter
	Vector3f getPosition();
	Vector3f getVelocity();
	Quaternionf getRotation();

private:
	DroneState currentState, oldState;
	DroneState savedState;

	DroneController* controller;

	Quaternionf slerpRot;
	PID_Quat pidRot;

	Quaternionf cam1RotGlobal, cam3RotGlobal;		//Camera Rotations

	Vector3f appliedRelMom;
	float appliedThrottle;
	float prop[NUM_PROP];				//Prop Speed
	Vector3f propPosGlobal[NUM_PROP];	//Prop Positions

	Blip blip;
	Camera cam1, cam3, camF;
	AudioHandler audio;
	Object collider, modelCase;
	Object modelProp[NUM_PROP];
	Object modelFris[NUM_PROP];

	void flip();
	void unstuck();

	void setTrails(bool enable);

	void resetHistory();

	void updateProps();

	void updateMomentum(const Quaternionf &desiredRot);
	void updateForce(const float &inputThrottle);

	void updateCameras();
	void updateMinimap();
	void updateAudioSources();
	void updateAudioListener();

	//Private Static Const
	static const float gravity;
	static const Quaternionf colliderRotLocal;
	static const Vector3f camDefaultOffsetLocal;

	//Private Static
	static float maxThrust, dragCoef;
	static Quaternionf cam1RotLocal, cam3RotLocal;
	static Vector3f propPosLocal[NUM_PROP];
};

