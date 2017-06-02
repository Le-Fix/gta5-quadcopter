#include "menuValueB.h"

menuValueB::menuValueB(bool _defaultValue, std::string _section, std::string _key, std::string _name)
	: menuValue(0, (int)_defaultValue, 1, 1, _section, _key, _name, "")
{
	updateRealValue();
}


menuValueB::~menuValueB()
{
}

std::string menuValueB::to_string()
{
	if (realValue)
	{
		return "On";
	}
	return "Off";
}

bool menuValueB::get()
{
	return realValue;
}

void menuValueB::updateRealValue()
{
	realValue = 1 == intValue;
}
