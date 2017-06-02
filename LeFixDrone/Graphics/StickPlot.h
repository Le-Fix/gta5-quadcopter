#pragma once

#include "Plot.h"
#include "..\Input\Gamepad.h"

class StickPlot :
	public Plot
{
public:
	StickPlot(Gamepad* _pad);
	~StickPlot();

	void setPad(Gamepad* _pad);

	void refreshData();
	void draw();

private:
	Gamepad* pad;
	float lx, ly, rx, ry;
};

