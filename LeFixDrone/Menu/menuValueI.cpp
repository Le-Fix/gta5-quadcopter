#include "menuValueI.h"

menuValueI::menuValueI(int _min, int _defaultValue, int _max, int _increment, std::string _section, std::string _key, std::string _name, std::string _unit)
	: menuValue(_min, _defaultValue, _max, _increment, _section, _key, _name, _unit)
{
	updateRealValue();
}


menuValueI::~menuValueI()
{
}

std::string menuValueI::to_string()
{
	return std::to_string(realValue) + unit;
}

int menuValueI::get()
{
	return realValue;
}

void menuValueI::updateRealValue()
{
	realValue = intValue;
}
