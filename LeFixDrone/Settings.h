#pragma once

#include <string>
#include "Menu\menuValueB.h"
#include "Menu\menuValueI.h"
#include "Menu\menuValueF.h"
#include "Menu\menuValueS.h"

class Settings
{
public:

	//AUDIO
	static menuValueS audioSub;
	static menuValueF audioVolume;

	//VISUAL
	static menuValueS visualSub;
	static menuValueB showCollider, showModel, showTrails, showStickCam;

	//GAMEPAD
	static menuValueS gamepadSub;
	static menuValueB gamepadVib, gamepadInvPitch;

	//CAMERA
	static menuValueS camSub, camD1Sub, camD3Sub, camDFSub, camC1Sub;
	static menuValueI camMode;
	static menuValueI camDrone1FOV, camDrone1Tilt, camDrone3FOV, camDrone3Tilt, camClone1CloseFOV, camClone1InfFOV;
	static menuValueF camDrone3YPos, camDrone3ZPos;

	//CONTROL
	static menuValueS controlSub;
	static menuValueF contRcRatePR, contRcRateY;
	static menuValueF contRateP, contRateR, contRateY;
	static menuValueF contExpoPR, contExpoY;

	//PID
	static menuValueS pidSub;
	static menuValueB pidEnable;
	static menuValueF pidP, pidI, pidD;

	//DRONE
	static menuValueS droneSub;
	static menuValueF droneMass, droneMaxRelLoad, droneMaxVel;
	static menuValueB drone3DFly, droneAcroMode;

	//PHYSICS
	static menuValueS physxSub;
	static menuValueF physxGScale;
	static menuValueB physxColl;

	//EXIT
	static menuValueS exitSub, exitHere, exitStart;

	//DEBUG
	static menuValueS debugSub;
	static menuValueI debugX;

	//SAVE LOAD PATH

	static void load();
	static void save();

	static std::string filePath;
};

