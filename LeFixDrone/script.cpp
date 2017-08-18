//Rotate Ped to get accurate bullet decals (tried, is strange)
//Map scale
//switch to fpv and animate goggles

#include "script.h"

#include "inc\main.h"

#include "AudioHandler.h"
#include "Clone.h"
#include "TimeCycleManager.h"
#include "Drone\Drone.h"
#include "Input\Gamepad.h"

#include <menu.h>

#include "Utils\UIutils.hpp"
#include "Utils\INIutils.hpp"

#include "Graphics\CurvePlot.h"
#include "Graphics\StickPlot.h"
#include "nativesExtended.h"

std::string modVersion = "v1.2";

NativeMenu::Menu menu;

Drone *drone = nullptr;
Clone *clone = nullptr;

Gamepad gamepad;

LeFix::eEXIT currentExitCode = LeFix::exitNo;

StickPlot stickPlot(&gamepad);
CurvePlot curvePlot(&gamepad);

void initialize()
{
	std::string path_settings_mod = GetCurrentModulePath() + "LeFixDrone\\settings_mod.ini";
	std::string path_settings_menu = GetCurrentModulePath() + "LeFixDrone\\settings_menu.ini";
	std::string audioPath50 = GetCurrentModulePath() + "LeFixDrone\\AudioLow.wav";
	std::string audioPath80 = GetCurrentModulePath() + "LeFixDrone\\AudioHigh.wav";

	menu.SetFiles(path_settings_menu);
	menu.RegisterOnMain(std::bind(onMenuEnter));
	menu.RegisterOnExit(std::bind(onMenuExit));
	menu.ReadSettings();

	//Menu
	menu.menuX = 0.13f;
	menu.menuY = 0.06f;

	//Settings
	Settings::SetFile(path_settings_mod);
	Settings::Load();
	Settings::Save();

	//Audio
	AudioHandler::initialize();
	AudioHandler::loadFile(audioPath50.c_str());
	AudioHandler::createBuffer(LeFix::mid);
	AudioHandler::loadFile(audioPath80.c_str());
	AudioHandler::createBuffer(LeFix::hgh);

	Drone::applyDragThrust(); //Static

	stickPlot.refreshData();
}

void ScriptMain()
{
	static bool boxFromModItself = false; //Don't overwrite existing helpbox

	initialize();
	while (true) {

		update();

		if (isAbleToStartFlight())	//Start Flight?
		{
			//Textbox checken
			if (!(UI::IS_HUD_COMPONENT_ACTIVE(eHudComponent::HudComponentHelpText) && !boxFromModItself)) //No other helptextbox visible
			{
				//showTextboxTop("Back to start location ~INPUT_FRONTEND_ACCEPT~~n~Teleport player to drone ~INPUT_FRONTEND_CANCEL~", true); //Match with gamepad class?
				showTextboxTop("Start Quadcopter Flight ~INPUT_FRONTEND_ACCEPT~", false);
				boxFromModItself = true;
				if (gamepad.button_accept) startFlight();
			}
			else {
				boxFromModItself = false; //Mod doesn't print box!
			}
		}
		else {
			boxFromModItself = false; //Mod doesn't print box!
		}
	}
}

void onMenuEnter() {
	Settings::Load();
	//float tscale = 0.05f;
	//GAMEPLAY::SET_TIME_SCALE(tscale);
	//AudioHandler::setTimeScale(tscale);
}
void onMenuExit() {
	Settings::Save();
	//float tscale = 1.0f;
	//GAMEPLAY::SET_TIME_SCALE(tscale);
	//AudioHandler::setTimeScale(tscale);
}

void update()
{
	//Input
	gamepad.refresh();

	//TimeCycle (ScreenEffects)
	static int count = 0;
	if (count == 20) {
		TimeCycleManager::update();
		count = 0;
	}
	count++;

	//No drone present?
	if (drone == nullptr) {

	}
	//Drone present?
	else
	{
		//Flight
		updateFlight();

		//Stickplot
		if (Settings::showStickCam) {
			stickPlot.refreshData();
			stickPlot.draw();
		}

		//Toggle Cameramode
		if (gamepad.button_cam)
		{
			Settings::camMode++;
			if (Settings::camMode > LeFix::camModeC1) Settings::camMode = 0;
			drone->applyCam();
			clone->refreshCamMode();
		}
		//CAM::SET_CAM_AFFECTS_AIMING() //TOMTOM

		//Fast Exit?
		if (
			PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()) ||
			PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE) ||
			GAMEPLAY::GET_MISSION_FLAG())
		{
			endFlightQuick();
			currentExitCode = LeFix::exitNo;
		}
		else if (currentExitCode == LeFix::exitNo)
		{
			updateMenu();
			if (currentExitCode != LeFix::exitNo)
			{
				menu.CloseMenu();
				onMenuExit();
				endFlight(currentExitCode == LeFix::exitStart);
				currentExitCode = LeFix::exitNo;
			}
		}
	}
	WAIT(0);
}

void updateFlight()
{
	//Disable Buttons and activites while flying drone
	disableFlightButtons();

	//Update Drone
	drone->update(gamepad);

	//Update Clone
	clone->update(drone->getPosition());

	//Update Ped (load world)
	ENTITY_X::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), drone->getPosition());
	ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), FALSE, FALSE);
	if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(PLAYER::PLAYER_PED_ID()))
	{
		MOBILE::DESTROY_MOBILE_PHONE();
	}
	WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), 0xA2719263, TRUE); //unarmed
}

void updateMenu()
{
	menu.CheckKeys();

	if (menu.CurrentMenu("mainmenu")) {
		menu.Title("Quadcopter");
		menu.Subtitle("v1.2 by LeFix");
		menu.FloatOption("Volume", Settings::audioVolume, 0.0f, 1.0f, 0.1f, { "Global volume for all mod-related sounds." });
		menu.MenuOption("Camera", "cameramenu", { "Settings for all camera modes." });
		menu.MenuOption("Control", "controlmenu", { "Settings for conversion of gamepad output to drone input." });
		menu.MenuOption("Drone", "dronemenu", { "Drone specific settings." });
		menu.MenuOption("Gamepad", "gamepadmenu", { "Gamepad/input device specific settings." });
		menu.MenuOption("Physics", "physxmenu", { "General physics settings." });
		menu.MenuOption("Visual", "visualmenu", { "Visibility, light trails..." });
		menu.MenuOption("Exit", "exitmenu", { "Exit drone mode here." });
	}

	if (menu.CurrentMenu("cameramenu")) {
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Camera");

		//Apply bools
		bool d = false;
		bool c = false;

		//Camera Mode
		if (menu.StringArray("Cam Mode", { "Drone 1st Person", "Drone 3rd Person", "Drone Follow", "Player Dynamic" }, Settings::camMode, { "Choose your camera mode and adjust its specific settings. It can be changed with the assigned button GTA V." }))
		{
			switch (Settings::camMode)
			{
			case LeFix::camModeD1:
			case LeFix::camModeD3:
			case LeFix::camModeDF:
				d = true;
				break;
			case LeFix::camModeC1:
				c = true;
				break;
			}
		}

		//Specific Camera Settings
		switch (Settings::camMode)
		{
			case LeFix::camModeD1 :
				d = d || menu.IntOption("Field of View", Settings::camDrone1FOV, 60, 120);
				d = d || menu.IntOption("Camera Tilt", Settings::camDrone1Tilt, -90, 90, 1, { "To compensate the attack angle and better match the actual flight direction. For racing around 30." });
				break;
			case LeFix::camModeD3:
				d = d || menu.IntOption("Field of View", Settings::camDrone3FOV, 60, 120);
				d = d || menu.IntOption("Camera Tilt", Settings::camDrone3Tilt, -90, 90, 1, { "To compensate the attack angle and better match the actual flight direction. For racing around 30." });
				d = d || menu.FloatOption("Camera Y Position", Settings::camDrone3YPos, -5.0f, 0.0f, 0.05f);
				d = d || menu.FloatOption("Camera Z Position", Settings::camDrone3ZPos, -2.0f, 2.0f, 0.05f);
				break;
			case LeFix::camModeDF:
				break;
			case LeFix::camModeC1:
				menu.IntOption("Field of View [Close]", Settings::camClone1CloseFOV, 45, 120, 1, { "The FOV is dynamic. Sets the the FOV at a reference distance of 2 meters." });
				menu.IntOption("Field of View [Far]", Settings::camClone1FarFOV, 0, 45, 1, { "The FOV is dynamic. Sets the the FOV at an infinite distance." });
				break;
		}

		//Apply Changes
		if(d) drone->applyCam();
		if(c) clone->refreshCamMode();
	}

	if (menu.CurrentMenu("controlmenu")) {
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Control");

		//Apply bools
		bool b = false;

		//Options
		b = b || menu.FloatOption("RC Rate PR",	Settings::contRcRatePR,	0.1f, 3.0f, 0.1f, { "Linear factor for Pitch and Roll." });
		b = b || menu.FloatOption("RC Rate Y",	Settings::contRcRateY,		0.1f, 3.0f, 0.1f, { "Linear factor for Yaw." });
		b = b || menu.FloatOption("Expo PR",	Settings::contExpoPR,		0.0f, 1.0f, 0.02f, { "Vary linear and cubic portion of curve which affects small inputs but doesn't the change max rotation velocity." });
		b = b || menu.FloatOption("Expo Y",		Settings::contExpoY,		0.0f, 1.0f, 0.02f, { "Vary linear and cubic portion of curve which affects small inputs but doesn't the change max rotation velocity." });
		b = b || menu.FloatOption("Rate P(itch)",Settings::contRateP,		0.0f, 0.98f, 0.02f, { "Greatly increases maximum rotation velocity without changing curve at low inputs alot.", "Also known as super expo." });
		b = b || menu.FloatOption("Rate R(oll)",Settings::contRateR,		0.0f, 0.98f, 0.02f, { "Greatly increases maximum rotation velocity without changing curve at low inputs alot.", "Also known as super expo." });
		b = b || menu.FloatOption("Rate Y(aw)",	Settings::contRateY,		0.0f, 0.98f, 0.02f, { "Greatly increases maximum rotation velocity without changing curve at low inputs alot.", "Also known as super expo." });
		
		//Apply changes
		if(b) curvePlot.refreshData();

		//Everytime
		curvePlot.draw();
	}
	
	if (menu.CurrentMenu("dronemenu")) {
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Drone");

		//Apply bools
		bool m = false;
		bool s = false;
		bool c = false;

		//Options
		m = m || menu.FloatOption("Mass",			Settings::droneMass,		 0.1f,  10.0f, 0.1f, { "Drone mass" });
		s = s || menu.FloatOption("Max rel. Load",	Settings::droneMaxRelLoad,	 0.0f,   5.0f, 0.1f, { "Maximum extra load the drone is capable to carry." });
		s = s || menu.FloatOption("Max Velocity",	Settings::droneMaxVel,		10.0f, 200.0f, 1.0f, { "Maximum horizontal velocity the drone can achieve. Implicitly determines the drag coefficient." });
		         menu.BoolOption("3D Flying",		Settings::drone3DFly,		{ "Enables downward/reverse thrust." });
				 menu.BoolOption("No Thrust Stick Down", Settings::droneNoThrustDown, { "No thrust stick position is stick down." });
		c = c || menu.BoolOption("Acro Mode",		Settings::droneAcroMode,	{ "Enables direct control mode especially for racing." });
		
		//Apply Changes
		if (m) drone->applyCollider();
		if (s) Drone::applyDragThrust();
		if (c) drone->applyController();
	}

	if (menu.CurrentMenu("gamepadmenu"))
	{
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Gamepad");

		//Options
		menu.BoolOption("Vibration", Settings::gamepadVib, { "Toggle gamepad vibration.", "(Heavy collisions)" });
		menu.BoolOption("Using inverted cam [GTA5]", Settings::gamepadInvPitch, { "Enabling inverted camera in the GTA5 options will invert the pitch input, this setting will invert it again." });
	}

	if (menu.CurrentMenu("physxmenu"))
	{
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Physics");

		//Options
		bool g = menu.FloatOption("Gravity Scale", Settings::physxGScale, 0.5f, 2.0f, 0.1f, { "Simple gravity multiplier. For fast outdoor flying >1 is probably more fun and for indoor flying <1 is probably easier." });
		bool c = menu.BoolOption("Collision", Settings::physxColl, { "Toggle drone collision." });
		menu.BoolOption("Use PID", Settings::pidEnable, {"The desired rotation of the drone is achieved by a PID Controller in real world. When disabled the rotation gets set, undermines physics."});

		//Apply Changes
		if (g) Drone::applyDragThrust();
		if (g || c)	drone->applyCollider();
	}

	if (menu.CurrentMenu("visualmenu"))
	{
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Visual");

		//Apply Bools
		bool v = false;

		//Options
		menu.BoolOption("Sticks", Settings::showStickCam, { "Prints the stick position on the bottom of the screen. Intended for screen capture." });
		v = v || menu.BoolOption("Trails", Settings::showTrails, { "Adds some particle effects at the prop positions." });
		v = v || menu.BoolOption("Collision Box Visible", Settings::showCollider, { "Visibility of the collision box model, which is does all the physics." });
		v = v || menu.BoolOption("Drone Model Visible", Settings::showModel, { "Visibility of the drone model, which is attached to the collsion box." });

		//Apply
		if (v) drone->applyVisual();
	}

	if (menu.CurrentMenu("exitmenu"))
	{
		//Title
		menu.Title("Quadcopter");
		menu.Subtitle("Exit");

		//Options
		if (menu.MenuOption("Go back to player", "dummy", { "Exit drone mode and go back to start location." }))
		{
			currentExitCode = LeFix::exitStart;
		}
		if (menu.MenuOption("Teleport player here", "dummy", { "Exit drone mode but teleport player to current location." }))
		{
			currentExitCode = LeFix::exitHere;
		}
	}

	menu.EndMenu();
}

bool isAbleToStartFlight()
{
	return ( ENTITY::GET_ENTITY_SPEED(PLAYER::PLAYER_PED_ID()) < 0.2f &&			//Not moving
		!ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) &&						//Not swimming
		UI::IS_HUD_COMPONENT_ACTIVE(eHudComponent::HudComponentWeaponWheel) &&		//Weapon wheel active
		PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()) &&								//On Foot
		!PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(PLAYER::PLAYER_PED_ID()) &&			//No Phone Call
		!GAMEPLAY::GET_MISSION_FLAG() );											//No active mission
}

void startFlight()
{
	//Fade out
	CAM::DO_SCREEN_FADE_OUT(400);
	WAIT_LONG(500);

	//Get reference
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	//Change PlayerPed
	ENTITY::SET_ENTITY_COLLISION(playerPed, FALSE, FALSE);
	UI::SET_BLIP_DISPLAY(UI::GET_MAIN_PLAYER_BLIP_ID(), 3);

	//Clone PlayerPed
	clone = new Clone(playerPed);

	//ADDITIONAL
	ENTITY::FREEZE_ENTITY_POSITION(playerPed, TRUE);
	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), TRUE);

	Quaternionf pedRot = ENTITY_X::GET_ENTITY_QUATERNION(playerPed);
	Vector3f pedPos = ENTITY_X::GET_ENTITY_COORDS(playerPed);

	Vector3f dronePos = pedPos + Vector3f(0.0f, 0.0f, -0.5f) + pedRot._transformVector(Vector3f(0.0, 1.0f, 0.0f)); //lower in front of player
	Vector3f droneVel = Vector3f(0.0f, 0.0f, 0.0f);
	Quaternionf droneRot = pedRot;

	drone = new Drone(dronePos, droneVel, droneRot);

	clone->refreshCamMode();

	//Fade in
	WAIT_LONG(300);
	CAM::DO_SCREEN_FADE_IN(400);
	TimeCycleManager::setTimecycleFadeOut("NG_filmic21", 7.0f);

	clone->setPedFreeze(true);
}
void endFlight(bool goBack)
{
	//Get reference
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	//Audio
	AUDIO::_PLAY_AMBIENT_SPEECH1(PLAYER::PLAYER_PED_ID(), "ROLLERCOASTER_CHAT_NORMAL", "SPEECH_PARAMS_FORCE");

	//Fade out
	CAM::DO_SCREEN_FADE_OUT(400);
	WAIT_LONG(500);

	//Use specific loacation
	float distance;
	if (goBack)
	{
		distance = (clone->getPosition() - drone->getPosition()).norm();

		//Move PlayerPed to Clone
		ENTITY_X::SET_ENTITY_COORDS(playerPed, clone->getPosition());
		ENTITY_X::SET_ENTITY_QUATERNION(playerPed, clone->getQuaternion());
	}
	else
	{
		distance = 0.0f;
		//Teleport Player to drone (already done)
	}

	//Delete clone
	delete clone;
	clone = nullptr;

	//Change PlayerPed
	ENTITY::SET_ENTITY_COLLISION(playerPed, TRUE, FALSE);
	ENTITY::SET_ENTITY_VISIBLE(playerPed, TRUE, FALSE);
	UI::SET_BLIP_DISPLAY(UI::GET_MAIN_PLAYER_BLIP_ID(), 2);

	//Additional
	UI::UNLOCK_MINIMAP_ANGLE();
	ENTITY::FREEZE_ENTITY_POSITION(playerPed, FALSE);
	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), FALSE);

	delete drone;
	drone = nullptr;

	//Wait for loading world
	DWORD waitTime = 500 + DWORD(distance);
	WAIT_LONG(waitTime);
	
	//Fade in
	CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, FALSE, FALSE);
	CAM::DO_SCREEN_FADE_IN(400);
}
void endFlightQuick()
{
	//Close Menu
	menu.CloseMenu();

	//Delete clone
	delete clone;
	clone = nullptr;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	//Change PlayerPed
	ENTITY::SET_ENTITY_COLLISION(playerPed, TRUE, FALSE);
	ENTITY::SET_ENTITY_VISIBLE(playerPed, TRUE, FALSE);
	UI::SET_BLIP_DISPLAY(UI::GET_MAIN_PLAYER_BLIP_ID(), 2);

	//Additional
	UI::UNLOCK_MINIMAP_ANGLE();
	ENTITY::FREEZE_ENTITY_POSITION(playerPed, FALSE);
	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), FALSE);

	delete drone;
	drone = nullptr;

	CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, FALSE, FALSE);
}

void disableFlightButtons()
{
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlCharacterWheel, TRUE);
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlSelectWeapon, TRUE);	 //TOMTOM
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlPhone, TRUE);
}

void WAIT_LONG(DWORD waitTime)
{
	static DWORD maxTickCount;
	maxTickCount = GetTickCount() + waitTime;
	do {
		WAIT(0);
		update();
	} while (GetTickCount() < maxTickCount);
}

