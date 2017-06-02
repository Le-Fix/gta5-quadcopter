#pragma once
#include "menuValue.h"
class menuValueB : public menuValue
{

public:
	menuValueB(bool _defaultValue, std::string _section, std::string _key, std::string _name);
	~menuValueB();

	std::string to_string();
	bool get();

protected:
	void updateRealValue();
	bool realValue;
};

