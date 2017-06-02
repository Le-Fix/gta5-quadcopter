#pragma once
#include "menuValue.h"
class menuValueS : public menuValue
{

public:
	menuValueS(std::string _name);
	~menuValueS();

	std::string to_string();

protected:
	void updateRealValue();
};

