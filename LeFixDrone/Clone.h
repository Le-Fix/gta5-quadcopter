#pragma once

#include "LeFixDrone.h"
#include "nativesExtended.h"

#include "AudioHandler.h"

class Clone
{
public:
	Clone(Ped original);
	~Clone();

	void update(Vector3f lookAt);
	void refreshSettingsDynamic();
	void refreshCamMode();
	void setPedFreeze(bool freeze);
	void playAnimations();

	Vector3f getPosition();
	Quaternionf getQuaternion();

private:
	Ped ped;
	Cam cam1;
	Blip blip;
	Object objController;

	float calculateFOV(float distance);
};

