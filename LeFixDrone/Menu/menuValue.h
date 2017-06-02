#pragma once

#include <string>

class menuValue
{
public:
	menuValue(int _min, int _defaultValue, int _max, int _increment, std::string _section, std::string _key, std::string _name, std::string _unit);
	~menuValue();

	virtual std::string to_string() = 0;
	
	void increase();
	void decrease();

	void load(std::string path);
	void save(std::string path);
	
	std::string getName();

protected:
	virtual void updateRealValue() = 0;
	int intValue;
	std::string unit;

private:
	int defaultValue;
	int min;
	int max;
	int increment;
	
	std::string section;
	std::string key;
	std::string name;
};

