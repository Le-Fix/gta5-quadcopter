#pragma once

#include "LeFixDrone.h"
#include "SHV\main.h"

#include "AudioHandler.h"
#include "Clone.h"
#include "TimeCycleManager.h"
#include "Drone\Drone.h"
#include "Input\Gamepad.h"

#include "Menu\menu.h"

#include "Utils\INIutils.hpp"
#include "Utils\UIutils.hpp"

#include <vector>

void ScriptMain();

static void initialize();

static void update();
static void updateFlight();

static void startFlight();
static void endFlight(bool goBack);
static void endFlightQuick();

static void disableDroneButtons();
static void disableMenuButtons();

static int  menu_process_main();
static void menu_process_audio();
static void menu_process_camera_main();
static void menu_process_camera_D1();
static void menu_process_camera_D3();
static void menu_process_camera_C1();
static void menu_process_control();
static void menu_process_drone();
static void menu_process_gamepad();
static void menu_process_physx();
static void menu_process_pid();
static void menu_process_visual();
static int  menu_process_exit();
static void menu_process_debug();

static void WAIT_LONG(DWORD waitTime);