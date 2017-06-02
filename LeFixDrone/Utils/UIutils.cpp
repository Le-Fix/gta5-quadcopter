#include "UIutils.hpp"

//Notification

void showNotification(std::string message) {
	int x;
	showNotification(x, message);
}

void showNotification(int &handle, std::string message) {
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(&message[0u]);
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
	UI::_SET_TEXT_ENTRY_2("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(&text[0u]);
	UI::_DRAW_SUBTITLE_TIMED(msec, 1);
}

//Textbox
void showTextboxTop(std::string text, bool beep) {
	UI::_SET_TEXT_COMPONENT_FORMAT("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(&text[0u]);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, false, beep, -1);
}

//Textbox
void showTextboxBottom(std::string text, bool beep) {
	UI::_SET_TEXT_COMPONENT_FORMAT("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(&text[0u]);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, false, beep, -1);
}