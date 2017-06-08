#pragma once

#include <string>

class Settings
{
public:

	//AUDIO
	static float audioVolume;

	//VISUAL
	static bool showCollider, showModel, showTrails, showStickCam;

	//GAMEPAD
	static bool gamepadVib, gamepadInvPitch;

	//CAMERA
	static int camMode;
	static int camDrone1FOV, camDrone1Tilt, camDrone3FOV, camDrone3Tilt, camClone1CloseFOV, camClone1FarFOV;
	static float camDrone3YPos, camDrone3ZPos;

	//CONTROL
	static float contRcRatePR, contRcRateY;
	static float contRateP, contRateR, contRateY;
	static float contExpoPR, contExpoY;

	//PID
	static bool pidEnable;
	static float pidP, pidI, pidD;

	//DRONE
	static float droneMass, droneMaxRelLoad, droneMaxVel;
	static bool drone3DFly, droneAcroMode;

	//PHYSICS
	static float physxGScale;
	static bool physxColl;

	//EXIT

	//DEBUG
	static int debugX;

	//SAVE LOAD PATH
	static std::string filePath;
};

