#include "menuValueF.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

menuValueF::menuValueF(float _min, float _defaultValue, float _max, float _increment, std::string _section, std::string _key, std::string _name, std::string _unit)
	: menuValue(int(_min/_increment), int(_defaultValue/_increment), int(_max/_increment), 1, _section, _key, _name, _unit)
{
	increment = _increment;
	updateRealValue();
}


menuValueF::~menuValueF()
{
}

std::string menuValueF::to_string()
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << realValue;       //convert double to string w fixed notation, hi precision
	std::string s = ss.str();                                    //output to std::string
	s.erase(s.find_last_not_of('0') + 1, std::string::npos);     //remove trailing 000s    (123.1200 => 123.12,  123.000 => 123.)
	if (s[s.size() - 1] == '.') s = s.substr(0, s.size() - 1);	 //dangling point
	return s + unit;
}

float menuValueF::get()
{
	return realValue;
}

void menuValueF::updateRealValue()
{
	realValue = increment*intValue;
}
