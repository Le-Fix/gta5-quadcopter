#include "menuValueS.h"

menuValueS::menuValueS(std::string _name)
	: menuValue(0, 0, 0, 0, "TMP", "tmp", _name, "")
{
}


menuValueS::~menuValueS()
{
}

std::string menuValueS::to_string()
{
	return "";
}

void menuValueS::updateRealValue()
{
	return; //Has no real value
}