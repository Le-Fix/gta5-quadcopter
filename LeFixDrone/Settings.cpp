#include "Settings.h"

std::string Settings::filePath;

//AUDIO
menuValueS Settings::audioSub = menuValueS("Audio");
menuValueF Settings::audioVolume = menuValueF(0.0f, 0.3f, 1.0f, 0.1f, "AUDIO", "volume", "Volume", "");

//VISUAL
menuValueS Settings::visualSub = menuValueS("Visual");
menuValueB Settings::showCollider		= menuValueB(false, "VISUAL", "collider", "Collision Box");
menuValueB Settings::showModel			= menuValueB(true,	"VISUAL", "model",    "Drone Model");
menuValueB Settings::showTrails			= menuValueB(true,	"VISUAL", "trails",   "Trails");
menuValueB Settings::showStickCam		= menuValueB(true, "VISUAL", "stickCam", "Stick Cam");
		   
//GAMEPAD  
menuValueS Settings::gamepadSub	= menuValueS("Gamepad");
menuValueB Settings::gamepadVib	= menuValueB(true, "GAMEPAD", "vibrate", "Vibration");
menuValueB Settings::gamepadInvPitch = menuValueB(false, "GAMEPAD", "invCamY", "GTA Inv. Cam");

//CAMERA   
menuValueS Settings::camSub			= menuValueS("Camera");
menuValueS Settings::camD1Sub		= menuValueS("Drone 1st Person");
menuValueS Settings::camD3Sub		= menuValueS("Drone 3rd Person");
menuValueS Settings::camDFSub		= menuValueS("Drone Follow");
menuValueS Settings::camC1Sub		= menuValueS("Player 1st Person");

menuValueI Settings::camMode		= menuValueI(0, 0, 3, 1, "CAM", "mode", "Mode", "");
menuValueI Settings::camDrone1FOV	= menuValueI( 60, 90, 120, 1, "CAMD1", "fov", "Field of View", " deg");
menuValueI Settings::camDrone3FOV	= menuValueI( 60, 90, 120, 1, "CAMD3", "fov", "Field of View", " deg");
menuValueI Settings::camDrone1Tilt	= menuValueI(-90, 30,  90, 1, "CAMD1", "tilt", "Tilt", " deg");
menuValueI Settings::camDrone3Tilt	= menuValueI(-90, 30,  90, 1, "CAMD3", "tilt", "Tilt", " deg");
menuValueF Settings::camDrone3YPos	= menuValueF(-5.0f, -2.0f, 0.0f, 0.05f, "CAMD3", "yPos", "Position Y", " m");
menuValueF Settings::camDrone3ZPos	= menuValueF(-2.0f,  0.0f, 2.0f, 0.05f, "CAMD3", "zPos", "Position Z", " m");
menuValueI Settings::camClone1CloseFOV	= menuValueI(45, 90, 120, 1, "CAMC1", "clsfov", "Near FOV", " deg");
menuValueI Settings::camClone1InfFOV	= menuValueI( 0, 10,  45, 1, "CAMC1", "inffov", "Far FOV", " deg");

//CONTROL  
menuValueS Settings::controlSub	= menuValueS("Control");
menuValueF Settings::contRcRatePR	= menuValueF(0.1f, 1.5f, 3.0f,  0.1f, "CONTROL", "rcRatePR", "RC Rate PR", ""); //linear factor
menuValueF Settings::contRcRateY	= menuValueF(0.1f, 1.5f, 3.0f,  0.1f, "CONTROL", "rcRateY",  "RC Rate Y", ""); //linear factor
menuValueF Settings::contExpoPR		= menuValueF(0.0f, 0.8f, 1.0f,  0.02f, "CONTROL", "rcExpoPR", "Expo PR", "");
menuValueF Settings::contExpoY		= menuValueF(0.0f, 0.8f, 1.0f,  0.02f, "CONTROL", "rcExpoY",  "Expo Y", "");
menuValueF Settings::contRateP		= menuValueF(0.0f, 0.5f, 0.98f, 0.02f, "CONTROL", "rateP", "Rate P", "");
menuValueF Settings::contRateR		= menuValueF(0.0f, 0.5f, 0.98f, 0.02f, "CONTROL", "rateR", "Rate R", "");
menuValueF Settings::contRateY		= menuValueF(0.0f, 0.3f, 0.98f, 0.02f, "CONTROL", "rateY", "Rate Y", "");
		   
//DRONE	   
menuValueS Settings::droneSub		= menuValueS("Drone");
menuValueF Settings::droneMass		= menuValueF( 0.1f,  0.8f,   2.0f, 0.05f, "DRONE", "mass",		"Mass", " kg");
menuValueF Settings::droneMaxRelLoad= menuValueF( 0.0f,  2.6f,   5.0f, 0.2f,  "DRONE", "maxRelLoad","Max rel. Load", "");
menuValueF Settings::droneMaxVel	= menuValueF(10.0f, 40.0f, 120.0f, 2.0f,  "DRONE", "maxSpeed",	"V Max", " m/s");
menuValueB Settings::drone3DFly		= menuValueB(false,                       "DRONE", "3dFly",		"3D Fly");
menuValueB Settings::droneAcroMode	= menuValueB(true,						  "DRONE", "acro",		"Acro Mode");

//PHYSICS
menuValueS Settings::physxSub		= menuValueS("Physics");
menuValueF Settings::physxGScale	= menuValueF(0.5f, 1.0f, 2.0f, 0.1f, "PHYSICS", "gScale", "G Scale", "");
menuValueB Settings::physxColl		= menuValueB(true, "PHYSICS", "collision", "Collision");

//PID
menuValueS Settings::pidSub = menuValueS("PID");
menuValueB Settings::pidEnable = menuValueB(true, "PID", "enable", "PID");
menuValueF Settings::pidP	= menuValueF(0.0f, 1.5f, 4.0f, 0.05f, "PID", "p", "Proportional", "");
menuValueF Settings::pidI	= menuValueF(0.0f, 0.0f, 1.0f, 0.01f, "PID", "i", "Integral", "");
menuValueF Settings::pidD	= menuValueF(0.0f, 0.8f, 2.0f, 0.05f, "PID", "d", "Derivative", "");

//EXIT
menuValueS Settings::exitSub		= menuValueS("Exit");
menuValueS Settings::exitHere		= menuValueS("Teleport Player Here");
menuValueS Settings::exitStart		= menuValueS("Go to Start Point");

//DEBUG
menuValueS Settings::debugSub = menuValueS("Debug");
menuValueI Settings::debugX = menuValueI(1, 1, 2, 1, "DEBUG", "x", "X", " ");

void Settings::load()
{
	//AUDIO
	audioVolume.load(filePath);

	//VISUAL
	showCollider.load(filePath);
	showModel.load(filePath);
	showTrails.load(filePath);
	showStickCam.load(filePath);

	//GAMEPAD
	gamepadVib.load(filePath);
	gamepadInvPitch.load(filePath);

	//CONTROL
	contRcRatePR.load(filePath);
	contRcRateY.load(filePath);
	contRateP.load(filePath);
	contRateR.load(filePath);
	contRateY.load(filePath);
	contExpoPR.load(filePath);
	contExpoY.load(filePath);

	//CAMERA
	//camMode.load(filePath);
	camDrone1FOV.load(filePath);
	camDrone3FOV.load(filePath);
	camDrone1Tilt.load(filePath);
	camDrone3Tilt.load(filePath);
	camDrone3YPos.load(filePath);
	camDrone3ZPos.load(filePath);
	camClone1CloseFOV.load(filePath);
	camClone1InfFOV.load(filePath);

	//DRONE
	droneMass.load(filePath);
	droneMaxRelLoad.load(filePath);
	droneMaxVel.load(filePath);
	drone3DFly.load(filePath);
	droneAcroMode.load(filePath);

	//PHYSICS
	physxGScale.load(filePath);
	physxColl.load(filePath);

	//PID
	pidEnable.load(filePath);
	pidP.load(filePath);
	pidI.load(filePath);
	pidD.load(filePath);
}

void Settings::save()
{
	//AUDIO
	audioVolume.save(filePath);

	//VISUAL
	showCollider.save(filePath);
	showModel.save(filePath);
	showTrails.save(filePath);
	showStickCam.save(filePath);

	//GAMEPAD
	gamepadVib.save(filePath);
	gamepadInvPitch.save(filePath);

	//CONTROL
	contRcRatePR.save(filePath);
	contRcRateY.save(filePath);
	contRateP.save(filePath);
	contRateR.save(filePath);
	contRateY.save(filePath);
	contExpoPR.save(filePath);
	contExpoY.save(filePath);

	//CAMERA
	//camMode.save(filePath);
	camDrone1FOV.save(filePath);
	camDrone3FOV.save(filePath);
	camDrone1Tilt.save(filePath);
	camDrone3Tilt.save(filePath);
	camDrone3YPos.save(filePath);
	camDrone3ZPos.save(filePath);
	camClone1CloseFOV.save(filePath);
	camClone1InfFOV.save(filePath);

	//DRONE
	droneMass.save(filePath);
	droneMaxRelLoad.save(filePath);
	droneMaxVel.save(filePath);
	drone3DFly.save(filePath);
	droneAcroMode.save(filePath);

	//PHYSICS
	physxGScale.save(filePath);
	physxColl.save(filePath);

	//PID
	pidEnable.save(filePath);
	pidP.save(filePath);
	pidI.save(filePath);
	pidD.save(filePath);
}