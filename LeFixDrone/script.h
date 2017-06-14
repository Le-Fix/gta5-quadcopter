#pragma once

#include "LeFixDrone.h"

#include <vector>

#include "GTAVMenuBase\menu.h"
#include "GTAVMenuBase\menucontrols.h"

void ScriptMain();

void initialize();

void update();
void updateFlight();
void updateMenu();

bool isAbleToStartFlight();

void startFlight();
void endFlight(bool goBack);
void endFlightQuick();

void disableFlightButtons();

void WAIT_LONG(DWORD waitTime);