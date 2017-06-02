#pragma once
#include "menuValue.h"
class menuValueI : public menuValue
{

public:
	menuValueI(int _min, int _defaultValue, int _max, int _increment, std::string _section, std::string _key, std::string _name, std::string _unit);
	~menuValueI();

	std::string to_string();
	int get();

protected:
	void updateRealValue();
	int realValue;
};

