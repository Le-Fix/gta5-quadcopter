#pragma once

#include "LeFixDrone.h"

#include <vector>

#include "menu.h"

void ScriptMain();

void initialize();

void update();
void updateFlight();
void updateMenu();

void onMenuEnter();
void onMenuExit();

bool isAbleToStartFlight();

void startFlight();
void endFlight(bool goBack);
void endFlightQuick();

void disableFlightButtons();

void WAIT_LONG(DWORD waitTime);