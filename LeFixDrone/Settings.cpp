#include "Settings.h"

#include "inc/enums.h"
#include "thirdparty/simpleini/SimpleIni.h"

std::string Settings::_filepath;

//AUDIO
float Settings::audioVolume = 0.3f;

//VISUAL
bool Settings::showCollider = false;
bool Settings::showModel = true;
bool Settings::showTrails = true;
bool Settings::showStickCam = true;

//GAMEPAD  
bool Settings::gamepadVib = true;
bool Settings::gamepadInvPitch = false;

//CAMERA

int Settings::camMode = 0;
int Settings::camDrone1FOV = 90;
int Settings::camDrone3FOV = 90;
int Settings::camDrone1Tilt = 30;
int Settings::camDrone3Tilt = 30;
float Settings::camDrone3YPos = -2.0f;
float Settings::camDrone3ZPos = 0.0f;
int Settings::camClone1CloseFOV = 90;
int Settings::camClone1FarFOV = 30;

//CONTROL
float Settings::contRcRatePR = 1.5f;
float Settings::contRcRateY = 1.5f;
float Settings::contExpoPR = 0.8f;
float Settings::contExpoY = 0.8f;
float Settings::contRateP = 0.5f;
float Settings::contRateR = 0.5f;
float Settings::contRateY = 0.3f;

//DRONE
float Settings::droneMass = 0.8f;
float Settings::droneMaxRelLoad = 2.6f;
float Settings::droneMaxVel = 40.0f;
bool Settings::drone3DFly = false;
bool Settings::droneAcroMode = true;
bool Settings::droneNoThrustDown = false;

//PHYSICS
float Settings::physxGScale = 1.0f;
bool Settings::physxColl = true;

//PID
bool Settings::pidEnable = true;
float Settings::pidP = 1.5f;
float Settings::pidI = 0.0f;
float Settings::pidD = 0.8f;

void Settings::SetFile(const std::string &filepath) {
	_filepath = filepath;
}

void Settings::Save() {
	CSimpleIniA settings;
	settings.SetUnicode();
	settings.LoadFile(_filepath.c_str());

	//AUDIO
	settings.SetDoubleValue("AUDIO", "volume", audioVolume);

	//CAMERA
	settings.SetLongValue("CAMERA", "mode", camMode);
	settings.SetLongValue("CAMERA", "d1FOV", camDrone1FOV);
	settings.SetLongValue("CAMERA", "d1Tilt", camDrone1Tilt);
	settings.SetLongValue("CAMERA", "d3FOV", camDrone3FOV);
	settings.SetLongValue("CAMERA", "d3Tilt", camDrone3Tilt);
	settings.SetLongValue("CAMERA", "c1CloseFOV", camClone1CloseFOV);
	settings.SetLongValue("CAMERA", "c1FarFOV", camClone1FarFOV);
	settings.SetDoubleValue("CAMERA", "d3YPos", camDrone3YPos);
	settings.SetDoubleValue("CAMERA", "d3ZPos", camDrone3ZPos);

	//CONTROL
	settings.SetDoubleValue("CONTROL", "RcRatePR", contRcRatePR);
	settings.SetDoubleValue("CONTROL", "RcRateY", contRcRateY);
	settings.SetDoubleValue("CONTROL", "RateP", contRateP);
	settings.SetDoubleValue("CONTROL", "RateR", contRateR);
	settings.SetDoubleValue("CONTROL", "RateY", contRateY);
	settings.SetDoubleValue("CONTROL", "ExpoPR", contExpoPR);
	settings.SetDoubleValue("CONTROL", "ExpoY", contExpoY);

	//DRONE
	settings.SetBoolValue("DRONE", "3dFly", drone3DFly);
	settings.SetBoolValue("DRONE", "acroMode", droneAcroMode);
	settings.SetDoubleValue("DRONE", "mass", droneMass);
	settings.SetDoubleValue("DRONE", "maxRelLoad", droneMaxRelLoad);
	settings.SetDoubleValue("DRONE", "maxVel", droneMaxVel);
	settings.SetBoolValue("DRONE", "zeroThrustDown", droneNoThrustDown);

	//GAMEPAD
	settings.SetBoolValue("GAMEPAD", "vibration", gamepadVib);
	settings.SetBoolValue("GAMEPAD", "invertedCam", gamepadInvPitch);

	//PID
	settings.SetBoolValue("PID", "enable", pidEnable);
	settings.SetDoubleValue("PID", "p", pidP);
	settings.SetDoubleValue("PID", "i", pidI);
	settings.SetDoubleValue("PID", "d", pidD);

	//PHYSICS
	settings.SetBoolValue("PHYSX", "collision", physxColl);
	settings.SetDoubleValue("PHYSX", "gscale", physxGScale);

	//VISUAL
	settings.SetBoolValue("VISUAL", "trails", showTrails);
	settings.SetBoolValue("VISUAL", "sticks", showStickCam);
	settings.SetBoolValue("VISUAL", "model", showModel);
	settings.SetBoolValue("VISUAL", "colli", showCollider);

	//Write to file
	settings.SaveFile(_filepath.c_str());
}

void Settings::Load() {
#pragma warning(push)
#pragma warning(disable: 4244) // Make everything doubles later...
	CSimpleIniA settings;
	settings.SetUnicode();
	settings.LoadFile(_filepath.c_str());

	//AUDIO
	audioVolume = settings.GetDoubleValue("AUDIO", "volume", 0.3);

	//CAMERA
	camMode = settings.GetLongValue("CAMERA", "mode", 0);
	camDrone1FOV = settings.GetLongValue("CAMERA", "d1FOV", 90);
	camDrone1Tilt = settings.GetLongValue("CAMERA", "d1Tilt", 30);
	camDrone3FOV = settings.GetLongValue("CAMERA", "d3FOV", 90);
	camDrone3Tilt = settings.GetLongValue("CAMERA", "d3Tilt", 30);
	camClone1CloseFOV = settings.GetLongValue("CAMERA", "c1CloseFOV", 100);
	camClone1FarFOV = settings.GetLongValue("CAMERA", "c1FarFOV", 30);
	camDrone3YPos = settings.GetDoubleValue("CAMERA", "d3YPos", -2.0);
	camDrone3ZPos = settings.GetDoubleValue("CAMERA", "d3ZPos", 0.1);

	//CONTROL
	contRcRatePR = settings.GetDoubleValue("CONTROL", "RcRatePR", 1.5);
	contRcRateY = settings.GetDoubleValue("CONTROL", "RcRateY", 1.5);
	contRateP = settings.GetDoubleValue("CONTROL", "RateP", 0.5);
	contRateR = settings.GetDoubleValue("CONTROL", "RateR", 0.5);
	contRateY = settings.GetDoubleValue("CONTROL", "RateY", 0.3);
	contExpoPR = settings.GetDoubleValue("CONTROL", "ExpoPR", 0.8);
	contExpoY = settings.GetDoubleValue("CONTROL", "ExpoY", 0.8);

	//DRONE
	drone3DFly = settings.GetBoolValue("DRONE", "3dFly", false);
	droneAcroMode = settings.GetBoolValue("DRONE", "acroMode", true);
	droneMass = settings.GetDoubleValue("DRONE", "mass", 0.8);
	droneMaxRelLoad = settings.GetDoubleValue("DRONE", "maxRelLoad", 2.6);
	droneMaxVel = settings.GetDoubleValue("DRONE", "maxVel", 40.0);
	droneNoThrustDown = settings.GetBoolValue("DRONE", "zeroThrustDown", false);

	//GAMEPAD
	gamepadVib = settings.GetBoolValue("GAMEPAD", "vibration", true);
	gamepadInvPitch = settings.GetBoolValue("GAMEPAD", "invertedCam", false);

	//PID
	pidEnable = settings.GetBoolValue("PID", "enable", true);
	pidP = settings.GetDoubleValue("PID", "p", 1.5);
	pidI = settings.GetDoubleValue("PID", "i", 0.0);
	pidD = settings.GetDoubleValue("PID", "d", 0.8);

	//PHYSICS
	physxColl = settings.GetBoolValue("PHYSX", "collision", true);
	physxGScale = settings.GetDoubleValue("PHYSX", "gscale", 1.0);

	//VISUAL
	showTrails = settings.GetBoolValue("VISUAL", "trails", true);
	showStickCam = settings.GetBoolValue("VISUAL", "sticks", true);
	showModel = settings.GetBoolValue("VISUAL", "model", true);
	showCollider = settings.GetBoolValue("VISUAL", "colli", false);
}