#include "INIutils.hpp"
#include <windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase; // MSVC specific, with other compilers use HMODULE from DllMain

std::string cachedModulePath;

std::string GetCurrentModulePath()
{
	if (cachedModulePath.empty())
	{
		// get module path
		char modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));
		GetModuleFileNameA((HMODULE)&__ImageBase, modPath, sizeof(modPath));
		for (size_t i = strlen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				modPath[i] = 0;
				break;
			}
		}
		cachedModulePath = modPath;
	}
	return cachedModulePath;
}

int readInt(std::string file, std::string section, std::string key, int standard, int min, int max)
{
	int iHelp = GetPrivateProfileInt(section.c_str(), key.c_str(), standard, file.c_str());
	if (iHelp < min || iHelp > max)
	{
		iHelp = standard;
	}
	return iHelp;
}

void saveInt(std::string file, std::string section, std::string key, int value, std::string comment)
{
	comment = std::to_string(value) + comment;
	WritePrivateProfileString(section.c_str(), key.c_str(), comment.c_str(), file.c_str());
}

std::string readString(std::string file, std::string section, std::string key, std::string defaultValue)
{
	std::string result = "aaaaaaaabbbbbbbbccccccccddddddddeeeeeeeeffffffffgggggggghhhhhhhh";
	GetPrivateProfileString(section.c_str(), key.c_str(), defaultValue.c_str(), (LPSTR)result.c_str(), 64, file.c_str());
	return result.c_str();
}

void saveString(std::string file, std::string section, std::string key, std::string value)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), file.c_str());
}