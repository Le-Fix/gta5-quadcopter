#pragma once
#include "menuValue.h"
class menuValueF : public menuValue
{

public:
	menuValueF(float _min, float _defaultValue, float _max, float _increment, std::string _section, std::string _key, std::string _name, std::string _unit);
	~menuValueF();

	std::string to_string();
	float get();

protected:
	void updateRealValue();
	float realValue;
private:
	float increment;
};

