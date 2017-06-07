#include "UIutils.hpp"

//Notification

void showNotification(std::string message) {
	int x;
	showNotification(x, message);
}

void showNotification(int &handle, std::string message) {
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&message[0u]);
	handle = UI::_DRAW_NOTIFICATION(false, false);
}

void removeNotification(int &handle)
{
	if (handle != 0) UI::_REMOVE_NOTIFICATION(handle);
}

void replaceNotification(int &handle, std::string message)
{
	removeNotification(handle);
	showNotification(handle, message);
}

//Subtitle
void showSubtitle(std::string text, int msec)
{
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&text[0u]);
	UI::END_TEXT_COMMAND_PRINT(msec, 1);
}

//Textbox
void showTextboxTop(std::string text, bool beep) {
	UI::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&text[0u]);
	UI::END_TEXT_COMMAND_DISPLAY_HELP(0, false, beep, -1);
}

//Textbox
void showTextboxBottom(std::string text, bool beep) {
	UI::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&text[0u]);
	UI::END_TEXT_COMMAND_DISPLAY_HELP(0, false, beep, -1);
}