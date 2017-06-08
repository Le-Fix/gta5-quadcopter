//Rotate Ped to get accurate bullet decals (tried, is strange)
//Map scale
//LightTrail
//end location
//switch to fpv and animate goggles


#include "script.h"

#include "LeFixDrone.h"
#include "SHV\main.h"

#include "AudioHandler.h"
#include "Clone.h"
#include "TimeCycleManager.h"
#include "Drone\Drone.h"
#include "Input\Gamepad.h"
#include "Input\keyboard.h"

#include "Utils\INIutils.hpp"
#include "Utils\UIutils.hpp"

#include "Graphics\CurvePlot.h"
#include "Graphics\StickPlot.h"
#include "nativesExtended.h"

std::string modVersion = "v1.2 pre";

NativeMenu::Menu menu;
NativeMenu::MenuControls menuControl;

Drone *drone = nullptr;
Clone *clone = nullptr;

Gamepad gamepad;

StickPlot stickPlot(&gamepad);
CurvePlot curvePlot(&gamepad);

void initialize()
{
	std::string path_settings_mod = GetCurrentModulePath() + "LeFixDrone\\settings_mod.ini";
	std::string path_settings_menu = GetCurrentModulePath() + "LeFixDrone\\settings_menu.ini";
	std::string audioPath50 = GetCurrentModulePath() + "LeFixDrone\\AudioLow.wav";
	std::string audioPath80 = GetCurrentModulePath() + "LeFixDrone\\AudioHigh.wav";

	//Menu
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuKey]		= str2key("SPACE");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuUp]		= str2key("UP");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuDown]	= str2key("DOWN");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuLeft]	= str2key("LEFT");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuRight]	= str2key("RIGHT");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuSelect]	= str2key("RETURN");
	menuControl.ControlKeys[NativeMenu::MenuControls::ControlType::MenuCancel]	= str2key("BACKSPACE");

	menu.LoadMenuTheme(std::wstring(path_settings_menu.begin(), path_settings_menu.end()).c_str());

	//Settings
	Settings::filePath = path_settings_mod;
	//Settings::load();
	//Settings::save();

	//Audio
	AudioHandler::initialize();
	AudioHandler::loadFile(audioPath50.c_str());
	AudioHandler::createBuffer(mid);
	AudioHandler::loadFile(audioPath80.c_str());
	AudioHandler::createBuffer(hgh);

	Drone::refreshSettingsStatic();

	//Gamepad
	gamepad = Gamepad();
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
			if (Settings::camMode > LeFixDrone::camModeC3) Settings::camMode = 0;
			drone->refreshCamMode();
			clone->refreshCamMode();
		}
		//CAM::SET_CAM_AFFECTS_AIMING() //TOMTOM

		//Fast Exit?
		if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) endFlightQuick();
		if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE)) endFlightQuick();
		if (GAMEPLAY::GET_MISSION_FLAG()) endFlightQuick();

		//Menu
		updateMenu();
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

void menuInit() {}
void menuClose() {}
void updateMenu()
{
	menu.CheckKeys(&menuControl, std::bind(menuInit), std::bind(menuClose));

	if (menu.CurrentMenu("mainmenu")) {
		menu.Title("LeFix Drone");

		menu.MenuOption("Audio", "audiomenu", { "All audio settings here." });
		menu.MenuOption("Camera", "cameramenu", { "Settings for all camera modes." });
		menu.MenuOption("Control", "controlmenu", { "Settings for conversion of gamepad", "output to drone input." });
		menu.MenuOption("Drone", "dronemenu", { "Drone specific settings." });
		menu.MenuOption("Gamepad", "gamepadmenu", { "Gamepad/input device specific settings." });
		menu.MenuOption("Physics", "physxmenu", { "General physics settings." });
		menu.MenuOption("Visual", "visualmenu", { "General physics settings." });
		menu.MenuOption("Exit", "exitmenu", { "Exit drone mode here." });
	}

	if (menu.CurrentMenu("cameramenu")) {
		menu.Title("Camera");
	}

	if (menu.CurrentMenu("controlmenu")) {
		menu.Title("Control");
		menu.FloatOption("RC Rate PR",	&Settings::contRcRatePR,	0.1f, 3.0f, 0.1f, { "Linear factor for Pitch and Roll." });
		menu.FloatOption("RC Rate Y",	&Settings::contRcRateY,		0.1f, 3.0f, 0.1f, { "Linear factor for Yaw." });
		menu.FloatOption("Expo PR",		&Settings::contExpoPR,		0.0f, 1.0f, 0.02f, { "Linear and Cubic parts.", "Effects small inputs.", "Doesn't change max rotation velocity." });
		menu.FloatOption("Expo Y",		&Settings::contExpoY,		0.0f, 1.0f, 0.02f, { "Linear factor for Yaw." });
	}
	
	if (menu.CurrentMenu("dronemenu")) {
		menu.Title("Drone");
		menu.FloatOption("Mass", &Settings::droneMass, 0.1f, 10.0f, 0.1f, { "Drone mass" });
		menu.FloatOption("Max rel. Load", &Settings::droneMaxRelLoad, 0.0f, 5.0f, 0.1f, { "Maximum extra load the drone","is capable to carry." });
		menu.FloatOption("Max Velocity", &Settings::droneMaxVel, 10.0f, 200.0f, 1.0f, { "Maximum horizontal velocity", "the drone can achieve.", "Implicitly sets drag coefficient" });
		menu.BoolOption("3D Flying", &Settings::drone3DFly, { "Enables downward/reverse thrust." });
		menu.BoolOption("Acro Mode", &Settings::droneAcroMode, { "Enables direct control mode", "especially for racing." });
	}
}

/*
void updateMenuIKT()
{
	menu.CheckKeys(&controls, std::bind(menuInit), std::bind(menuClose));

	if (menu.CurrentMenu("mainmenu")) {
		menu.Title("LeFix Drone");

		int shiftModeTemp = settings.ShiftMode;
		std::vector<std::string> gearboxModes = {
			"Sequential",
			"H-pattern",
			"Automatic"
		};

		menu.StringArray("Gearbox", gearboxModes, &shiftModeTemp, { "Choose your gearbox!" });
		if (shiftModeTemp != settings.ShiftMode) {
			settings.ShiftMode = shiftModeTemp;
			setShiftMode(shiftModeTemp);
		}

		menu.MenuOption("Mod options", "optionsmenu", { "Find all mod-related settings here!" });
		menu.MenuOption("Controls", "controlsmenu", { "For your keyboard and controller." });
		menu.MenuOption("Wheel Options", "wheelmenu", { "Set up your steering wheel." });
		menu.MenuOption("HUD Options", "hudmenu", { "Wanna move HUD elements around or",
			"disable things? Imperial or metric?" });
		//menu.MenuOption("Menu Options", "menumenu"); 
		menu.MenuOption("Debug", "debugmenu", { "Technical details and options." });

		int activeIndex = 0;
		std::string activeInputName;
		switch (controls.PrevInput) {
		case ScriptControls::Keyboard:
			activeInputName = "Keyboard";
			break;
		case ScriptControls::Controller:
			activeInputName = "Controller";
			break;
		case ScriptControls::Wheel:
			activeInputName = "Wheel";
			break;
		}
		std::vector<std::string> active = { activeInputName };
		menu.StringArray("Active input", active, &activeIndex,
		{ "Active input is automatically detected." });

		int versionIndex = 0;
		std::vector<std::string> version = { DISPLAY_VERSION };
		menu.StringArray("Version", version, &versionIndex,
		{ "Thank you for using this mod!","- ikt" });
	}

	if (menu.CurrentMenu("optionsmenu")) {
		menu.Title("Mod options");
		if (menu.BoolOption("Engine Damage", &settings.EngDamage,
		{ "Damage the engine when over-revving and","when mis-shifting." })) {
		}
		if (menu.BoolOption("Engine Stalling", &settings.EngStall,
		{ "Stall the engine when the wheel speed gets"," too low" })) {
		}
		if (menu.BoolOption("Engine Braking", &settings.EngBrake,
		{ "Help the car braking by slowing down more","at high RPMs" })) {
		}
		if (menu.BoolOption("Clutch Bite", &settings.ClutchCatching,
		{ "Simulate clutch biting action and clutch","interaction at near-stop speeds." })) {
		}
		if (menu.BoolOption("Clutch Shift (S)", &settings.ClutchShiftingS,
		{ "Require holding the clutch to shift", "in sequential mode." })) {
		}
		if (menu.BoolOption("Clutch Shift (H)", &settings.ClutchShiftingH,
		{ "Require holding the clutch to shift", "in H-pattern mode." })) {
		}
		if (menu.BoolOption("Default Neutral", &settings.DefaultNeutral,
		{ "The car will be in neutral when you get in." })) {
		}
		if (menu.MenuOption("Fine-tuning", "finetuneoptionsmenu",
		{ "Fine-tune the parameters above" })) {
		}
		if (menu.MenuOption("Misc. options", "miscoptionsmenu",
		{ "Options that don't really have to do with","the gearbox simulation." })) {
		}
	}

	if (menu.CurrentMenu("miscoptionsmenu")) {
		menu.Title("Misc. options");
		if (menu.BoolOption("Simple Bike", &settings.SimpleBike,
		{ "Disables bike engine stalling and the","clutch bite simulation." })) {
		}
		if (menu.BoolOption("Hill gravity workaround", &settings.HillBrakeWorkaround,
		{ "Gives the car a push to overcome","the games' default brakes at a stop." })) {
		}
		if (menu.BoolOption("Auto gear 1", &settings.AutoGear1,
		{ "Automatically switch to first gear","when the car reaches a standstill." })) {
		}
		if (menu.BoolOption("Auto look back", &settings.AutoLookBack,
		{ "Automatically look back whenever in","reverse gear." })) {
		}
		if (menu.BoolOption("Clutch + throttle start", &settings.ThrottleStart,
		{ "Allow to start the engine by pressing","clutch and throttle, like in DiRT Rally." })) {
		}
	}
}
*/

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

	drone->refreshCamMode();
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

