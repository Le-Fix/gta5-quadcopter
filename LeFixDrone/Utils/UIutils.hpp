#pragma once

#include "SHV\natives.h"
#include "SHV\types.h"
#include <string>

//Notification
void showNotification(std::string);
void showNotification(int&, std::string);
void removeNotification(int&);
void replaceNotification(int&, std::string);

//Subtitle
void showSubtitle(std::string, int );

//TextboxTop
void showTextboxTop(std::string, bool);

void showTextboxBottom(std::string text, bool beep);