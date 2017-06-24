#pragma once

#include "inc\natives.h"
#include "inc\enums.h"
#include "..\Settings.h"

class Gamepad
{

public:
	Gamepad() {}
	~Gamepad() {}

	bool button_unstuck;
	bool button_flip;
	bool button_cam;
	bool button_accept;
	bool button_cancel;

	bool button_up;
	bool button_down;
	bool button_right;
	bool button_left;

	float stick_left_x;
	float stick_left_y;
	float stick_right_x;
	float stick_right_y;

	void refresh();
};


















//#pragma once
//
//#include <Windows.h>
//#include <XInput.h>
//
//#pragma comment(lib, "XInput.lib")
//
//#include <cmath>
//
//class Gamepad
//{
//private:
//	int cId;
//	XINPUT_STATE state;
//
//	bool isVibrating = false;
//	DWORD vibrateStopTime;
//
//	void vibrate(int leftVal, int rightVal);
//
//	static bool gamepadVib;
//	static float deadzoneX;
//	static float deadzoneY;
//
//public:
//	Gamepad() {}
//	~Gamepad() {}
//
//	static void setDeadzone(float dzX, float dzY);
//
//	void initiateVibration(float intensity, DWORD duration);
//
//	float leftStickX;
//	float leftStickY;
//	float rightStickX;
//	float rightStickY;
//	float leftTrigger;
//	float rightTrigger;
//
//	int  GetPort();
//	XINPUT_GAMEPAD *GetState();
//	bool CheckConnection();
//	bool Refresh();
//	bool IsPressed(WORD);
//};