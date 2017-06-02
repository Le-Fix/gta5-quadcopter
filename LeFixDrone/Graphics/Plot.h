#pragma once

#include "../nativesExtended.h"
#include "ColorRGBA.h"

class Plot
{
public:
	Plot();
	~Plot();

	virtual void refreshData() = 0;
	virtual void draw() = 0;

	//id?

};