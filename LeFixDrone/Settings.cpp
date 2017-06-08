#include "Settings.h"

std::string Settings::filePath;

//AUDIO
float Settings::audioVolume = 0.3f;

//VISUAL
bool Settings::showCollider		= false;
bool Settings::showModel		= true;
bool Settings::showTrails		= true;
bool Settings::showStickCam		= true;
		   
//GAMEPAD  
bool Settings::gamepadVib		= true;
bool Settings::gamepadInvPitch	= false;

//CAMERA

int Settings::camMode			= 0;
int Settings::camDrone1FOV		= 90;
int Settings::camDrone3FOV		= 90;
int Settings::camDrone1Tilt		= 30;
int Settings::camDrone3Tilt		= 30;
float Settings::camDrone3YPos	= -2.0f;
float Settings::camDrone3ZPos	=  0.0f;
int Settings::camClone1CloseFOV	= 90;
int Settings::camClone1FarFOV	= 10;

//CONTROL
float Settings::contRcRatePR	= 1.5f;
float Settings::contRcRateY		= 1.5f;
float Settings::contExpoPR		= 0.8f;
float Settings::contExpoY		= 0.8f;
float Settings::contRateP		= 0.5f;
float Settings::contRateR		= 0.5f;
float Settings::contRateY		= 0.3f;
		   
//DRONE
float Settings::droneMass		= 0.8f;
float Settings::droneMaxRelLoad	= 2.6f;
float Settings::droneMaxVel		= 40.0f;
bool Settings::drone3DFly		= false;
bool Settings::droneAcroMode	= true;

//PHYSICS
float Settings::physxGScale	= 1.0f;
bool Settings::physxColl	= true;

//PID
bool Settings::pidEnable = true;
float Settings::pidP	= 1.5f;
float Settings::pidI	= 0.0f;
float Settings::pidD	= 0.8f;

//EXIT

//DEBUG
int Settings::debugX = 1;