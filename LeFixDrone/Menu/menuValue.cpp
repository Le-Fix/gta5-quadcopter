#include "menuValue.h"
#include "..\Utils\INIutils.hpp"

menuValue::menuValue(int _min, int _defaultValue, int _max, int _increment, std::string _section, std::string _key, std::string _name, std::string _unit)
{
	min = _min;
	defaultValue = _defaultValue;
	intValue = _defaultValue;
	max = _max;
	increment = _increment;
	section = _section;
	key = _key;
	name = _name;
	unit = _unit;
}
menuValue::~menuValue()
{

}

void menuValue::increase()
{
	intValue += increment;
	if (intValue > max) intValue = min;
	updateRealValue();
}

void menuValue::decrease()
{
	intValue -= increment;
	if (intValue < min) intValue = max;
	updateRealValue();
}

void menuValue::load(std::string path)
{
	intValue = readInt(path, section, key, defaultValue, min, max);
	updateRealValue();
}

void menuValue::save(std::string path)
{
	saveInt(path, section, key, intValue, ";");
}

std::string menuValue::getName()
{
	return name;
}