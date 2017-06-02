#pragma once

#define NUM_POINTS 120
//There's a limit of rectangles on screen around 400

#include "Plot.h"
#include "..\Input\Gamepad.h"
#include "..\Steuerung.h"

class CurvePlot :
	public Plot
{
public:
	CurvePlot(Gamepad* _pad);
	~CurvePlot();

	void refreshData();
	void draw();

private:

	Gamepad* pad;
	float current[6];

	enum data {ePv, eRv, eYv, ePi, eRi, eYi};
	float val[6][NUM_POINTS];
	float max;
	int maxPitch, maxRoll, maxYaw;
	bool critical;
};

