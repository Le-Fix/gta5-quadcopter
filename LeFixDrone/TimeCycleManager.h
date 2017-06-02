#pragma once
#include "SHV\types.h"
#include "SHV\natives.h"

#include "Utils\UIutils.hpp"

class TimeCycleManager
{
public:
	TimeCycleManager();
	~TimeCycleManager();

	static void setTimecycleFadeOut(char* modifierName, float _duration);
	static void setTimecycleFadeIn(char* modifierName, float _duration);
	static void update();

private:

	static bool isFadingOut;
	static bool isFadingIn;
	static float strength;
	static DWORD lastUpdate;
	static float duration;
};

