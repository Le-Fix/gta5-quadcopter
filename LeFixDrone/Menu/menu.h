#pragma once

#include "SHV\natives.h"
#include "menuValue.h"
#include <string>

void draw_caption(std::string caption, float lineWidth);
void draw_data_line(menuValue& data, int index, float lineWidth, bool active);
void draw_menu_line(std::string name, std::string value, int index, float lineWidth, bool active);

void menu_beep();