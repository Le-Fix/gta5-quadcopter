#include "TimeCycleManager.h"

bool TimeCycleManager::isFadingOut;
bool TimeCycleManager::isFadingIn;
float TimeCycleManager::strength;
DWORD TimeCycleManager::lastUpdate;
float TimeCycleManager::duration;

TimeCycleManager::TimeCycleManager()
{
}


TimeCycleManager::~TimeCycleManager()
{
}

void TimeCycleManager::setTimecycleFadeOut(char* modifierName, float _duration)
{
	duration = _duration; //to fast
	strength = 1.0f;
	isFadingIn = false;
	isFadingOut = true;
	lastUpdate = GetTickCount();
	GRAPHICS::SET_TIMECYCLE_MODIFIER(modifierName);
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(strength);
}

void TimeCycleManager::setTimecycleFadeIn(char* modifierName, float _duration)
{
	duration = _duration; //to fast
	strength = 0.0f;
	isFadingIn = false;
	isFadingOut = true;
	lastUpdate = GetTickCount();
	GRAPHICS::SET_TIMECYCLE_MODIFIER(modifierName);
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(strength);
}

void TimeCycleManager::update()
{
	if (isFadingIn)
	{
		strength += 0.001f * (GetTickCount() - lastUpdate) / duration;
		if (strength > 1.0f)
		{
			strength = 1.0f;
			isFadingIn = false;
		}
		GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(strength);
		lastUpdate = GetTickCount();
	}
	else if (isFadingOut)
	{
		strength -= 0.001f * (GetTickCount() - lastUpdate) / duration;
		if (strength < 0.0f)
		{
			strength = 0.0f;
			isFadingOut = false;
		}
		GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(strength);
		lastUpdate = GetTickCount();
	}
}