#pragma once

#include <string>

// returns module load path with trailing slash
std::string GetCurrentModulePath();

int readInt(std::string file, std::string section, std::string key, int standard, int min, int max);

void saveInt(std::string file, std::string section, std::string key, int value, std::string comment);

std::string readString(std::string file, std::string section, std::string key, std::string defaultValue);

void saveString(std::string file, std::string section, std::string key, std::string value);