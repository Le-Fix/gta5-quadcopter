//Rotate Ped to get accurate bullet decals (tried, is strange)
//Map scale
//LightTrail
//end location
//switch to fpv and animate goggles


#include "script.h"

#include "Graphics\CurvePlot.h"
#include "Graphics\StickPlot.h"
#include "nativesExtended.h"

std::string modVersion = "v1.1";

Drone *drone = nullptr;
Clone *clone = nullptr;

Gamepad gamepad;
StickPlot stickPlot(&gamepad);
CurvePlot curvePlot(&gamepad);

void initialize()
{
	std::string path = GetCurrentModulePath() + "LeFixDrone\\settings.ini";
	std::string audioPath50 = GetCurrentModulePath() + "LeFixDrone\\AudioLow.wav";
	std::string audioPath80 = GetCurrentModulePath() + "LeFixDrone\\AudioHigh.wav";

	//Settings
	Settings::filePath = path;
	Settings::load();
	Settings::save();

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
	initialize();
	while (true) {

		update();
		//No drone present?
		if (drone == nullptr) {

			static bool boxFromModItself = false; //Don't overwrite existing helpbox

			//Start Flight?
			if (ENTITY::GET_ENTITY_SPEED(PLAYER::PLAYER_PED_ID()) < 0.2f &&					//Not moving
				!ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) &&						//Not swimming
				UI::IS_HUD_COMPONENT_ACTIVE(eHudComponent::HudComponentWeaponWheel) &&		//Weapon wheel active
				PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()) &&								//On Foot
				!PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(PLAYER::PLAYER_PED_ID()) &&			//No Phone Call
				!GAMEPLAY::GET_MISSION_FLAG())												//No active mission
			{

				//Textbox checken
				if (!(UI::IS_HUD_COMPONENT_ACTIVE(eHudComponent::HudComponentHelpText) && !boxFromModItself)) //No other helptextbox visible
				{
					//showTextboxTop("Back to start location ~INPUT_FRONTEND_ACCEPT~~n~Teleport player to drone ~INPUT_FRONTEND_CANCEL~", true); //Match with gamepad class?
					showTextboxTop("Start Quadcopter Flight ~INPUT_FRONTEND_ACCEPT~", false);
					boxFromModItself = true;
					if(gamepad.button_accept) startFlight();
				}
				else {
					boxFromModItself = false; //Mod doesn't print box!
				}
			}
			else {
				boxFromModItself = false; //Mod doesn't print box!
			}
		}
		//Drone present?
		else
		{
			//Toggle Menu?
			if (gamepad.button_accept) {
				int exit = menu_process_main();
				Settings::save();
				switch (exit)
				{
				case exitNo: WAIT_LONG(200);  break;
				case exitStart: endFlight(true); break;
				case exitHere: endFlight(false); break;
				}
			}

			//Toggle Cameramode
			if (gamepad.button_cam)
			{
				Settings::camMode.increase();
				drone->refreshCamMode();
				clone->refreshCamMode();
			}
			//CAM::SET_CAM_AFFECTS_AIMING() //TOMTOM

			//Fast Exit?
			if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) endFlightQuick();
			if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE)) endFlightQuick();
			if (GAMEPLAY::GET_MISSION_FLAG()) endFlightQuick();
		}
		WAIT(0);
	}
}

void update()
{
	//Input source
	gamepad.refresh();

	static int count = 0;
	if (count == 20)
	{
		//TimeCycle (ScreenEffects)
		TimeCycleManager::update();
		count = 0;
	}
	count++;

	//Flight
	if (drone != nullptr) {
		updateFlight();
		if (Settings::showStickCam.get()) {
			stickPlot.refreshData();
			stickPlot.draw();
		}
	}
}

void updateFlight()
{
	//Disable Buttons and activites while flying drone
	disableDroneButtons();

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

void disableMenuButtons()		//Disable all actions which are initiated by gamepad buttons used for navigating the menu
{
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlCharacterWheel, TRUE);	// Down
}
void disableDroneButtons()
{
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlCharacterWheel, TRUE);
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlSelectWeapon, TRUE);	 //TOMTOM
	CONTROLS::DISABLE_CONTROL_ACTION(2, eControl::ControlPhone, TRUE);
}

int menu_process_main()
{
	static int index = 0;

	std::string caption = "LeFix Drone " + modVersion;
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::audioSub);
	data.push_back(&Settings::camSub);
	data.push_back(&Settings::controlSub);
	data.push_back(&Settings::droneSub);
	data.push_back(&Settings::gamepadSub);
	data.push_back(&Settings::physxSub);
	data.push_back(&Settings::visualSub);
	data.push_back(&Settings::exitSub);
	//data.push_back(&Settings::debugSub); //INDEV

	data.shrink_to_fit();

	DWORD waitTime = 500;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Return
		if (gamepad.button_cancel) return exitNo;
		//Submenu
		if (gamepad.button_accept)
		{
			if (data.at(index) == &Settings::audioSub)		menu_process_audio();
			if (data.at(index) == &Settings::camSub)		menu_process_camera_main();
			if (data.at(index) == &Settings::controlSub)	menu_process_control();
			if (data.at(index) == &Settings::droneSub)		menu_process_drone();
			if (data.at(index) == &Settings::gamepadSub)	menu_process_gamepad();
			if (data.at(index) == &Settings::physxSub)		menu_process_physx();if (data.at(index) == &Settings::visualSub)		menu_process_visual();
			if (data.at(index) == &Settings::exitSub)
			{
				int exitCode = menu_process_exit();
				if (exitCode != exitNo) return exitCode;
			}
			if (data.at(index) == &Settings::debugSub)		menu_process_debug(); //INDEV
			waitTime += 200;
		}
	}
}
void menu_process_audio()
{
	static int index = 0;

	std::string caption = "Audio";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::audioVolume);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_camera_main()
{
	int index = Settings::camMode.get();

	std::string caption = "Camera";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	//Has to be same order as eCamMode enum
	data.push_back(&Settings::camD1Sub);
	data.push_back(&Settings::camD3Sub);
	data.push_back(&Settings::camDFSub);
	data.push_back(&Settings::camC1Sub);
	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			while (Settings::camMode.get() != index) Settings::camMode.increase();

			menu_beep();
			drone->refreshCamMode();
			clone->refreshCamMode();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;

			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;

			waitTime += 100;
		}
		//Cam submenu
		if (gamepad.button_accept)
		{
			if (data.at(index) == &Settings::camD1Sub)		menu_process_camera_D1();
			if (data.at(index) == &Settings::camD3Sub)		menu_process_camera_D3();
			if (data.at(index) == &Settings::camC1Sub)		menu_process_camera_C1();
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_camera_D1()
{
	static int index = 0;

	std::string caption = "Drone First Person";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::camDrone1FOV);
	data.push_back(&Settings::camDrone1Tilt);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_camera_D3()
{
	static int index = 0;

	std::string caption = "Drone Third Person";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::camDrone3FOV);
	data.push_back(&Settings::camDrone3Tilt);
	data.push_back(&Settings::camDrone3YPos);
	data.push_back(&Settings::camDrone3ZPos);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_camera_C1()
{
	static int index = 0;

	std::string caption = "Player First Person";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::camClone1CloseFOV);
	data.push_back(&Settings::camClone1InfFOV);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_control()
{
	static int index = 0;

	std::string caption = "Control";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::contRcRatePR);
	data.push_back(&Settings::contRcRateY);
	data.push_back(&Settings::contExpoPR);
	data.push_back(&Settings::contExpoY);
	data.push_back(&Settings::contRateP);
	data.push_back(&Settings::contRateR);
	data.push_back(&Settings::contRateY);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			curvePlot.refreshData();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			curvePlot.draw();
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_drone()
{
	static int index = 0;

	std::string caption = "Drone";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::droneMass);
	data.push_back(&Settings::droneMaxRelLoad);
	data.push_back(&Settings::droneMaxVel);
	data.push_back(&Settings::drone3DFly);
	data.push_back(&Settings::droneAcroMode);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_gamepad()
{
	static int index = 0;

	std::string caption = "Gamepad";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::gamepadVib);
	data.push_back(&Settings::gamepadInvPitch);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0) menu_beep();

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_physx()
{
	static int index = 0;

	std::string caption = "Physics";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::physxGScale);
	data.push_back(&Settings::physxColl);
	data.push_back(&Settings::pidSub);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Submenu
		if (gamepad.button_accept)
		{
			if (data.at(index) == &Settings::pidSub)		menu_process_pid();
			waitTime += 200;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_pid()
{
	static int index = 0;

	std::string caption = "PID";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::pidEnable);
	data.push_back(&Settings::pidP);
	data.push_back(&Settings::pidI);
	data.push_back(&Settings::pidD);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			drone->refreshSettingsDynamic();
			menu_beep();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
void menu_process_visual()
{
	static int index = 0;

	std::string caption = "Visual";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::showCollider);
	data.push_back(&Settings::showModel);
	data.push_back(&Settings::showTrails);
	data.push_back(&Settings::showStickCam);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
}
int menu_process_exit()
{
	static int index = 0;

	std::string caption = "Exit";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::exitStart);
	data.push_back(&Settings::exitHere);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
			Drone::refreshSettingsStatic();
			drone->refreshSettingsDynamic();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Exit
		if (gamepad.button_accept)
		{
			if (data.at(index) == &Settings::exitStart)		return exitStart;
			if (data.at(index) == &Settings::exitHere)		return exitHere;
		}
		//Mainmenu
		if (gamepad.button_cancel) return exitNo;
	}
}

void menu_process_debug()
{
	static int index = 0;

	std::string caption = "Debug";
	const float lineWidth = 230.0;

	std::vector<menuValue*> data;

	data.push_back(&Settings::debugX);

	data.shrink_to_fit();

	DWORD waitTime = 200;
	while (true)
	{
		if (waitTime != 0)
		{
			menu_beep();
		}

		// Pause after active line switch or changed value
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// Draw Menu
			draw_caption(caption, lineWidth);
			for (int i = 0; i < data.size(); i++) draw_data_line(*data.at(i), i, lineWidth, i == index);
			disableMenuButtons(); //Disable all actions which are initiated by buttons used for navigating the menu
			WAIT_LONG(0);
			showTextboxBottom("Start Quadcopter Flight ~INPUT_FRONTEND_ACCEPT~", false);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		//Increase
		if (gamepad.button_right)
		{
			waitTime += 100;
			(*data.at(index)).increase();
		}
		//Decrease
		if (gamepad.button_left)
		{
			(*data.at(index)).decrease();
			waitTime += 100;
		}
		//Move up
		if (gamepad.button_up)
		{
			if (index == 0) index = (int)data.size();
			index--;
			waitTime += 100;
		}
		//Move down
		if (gamepad.button_down)
		{
			index++;
			if (index == data.size()) index = 0;
			waitTime += 100;
		}
		//Mainmenu
		if (gamepad.button_cancel) break;
	}
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