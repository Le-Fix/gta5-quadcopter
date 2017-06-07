#include "StickPlot.h"



StickPlot::StickPlot(Gamepad* _pad) : pad(_pad)
{
}

StickPlot::~StickPlot()
{
}

void StickPlot::setPad(Gamepad* _pad)
{
	pad = _pad;
}

void StickPlot::refreshData()
{
	if (pad == nullptr) return;
	//MAKE SURE POINTER IS VALID!
	lx = pad->stick_left_x;
	ly = pad->stick_left_y;
	rx = pad->stick_right_x;
	ry = pad->stick_right_y;
}

void StickPlot::draw()
{
	static ColorRGBA colDot = ColorRGBA(200, 200, 200, 200);
	static ColorRGBA colRct = ColorRGBA(0, 0, 0, 80);
	static ColorRGBA colBrd = ColorRGBA(0, 0, 0, 120);

	static float sizeDot = 0.01f;
	static float sizeRct = 0.1f;
	static float sizeGap = 0.01f;

	static float posX = 0.5f;
	static float posY = 0.94f;

	float ratio = GRAPHICS::_GET_ASPECT_RATIO(FALSE);

	//Draw Border
	GRAPHICS_X::DRAW_RECT(posX, posY - 0.5f*(sizeRct + sizeGap), (2 * sizeRct + 3 * sizeGap) / ratio, sizeGap, colBrd); //Top
	GRAPHICS_X::DRAW_RECT(posX, posY + 0.5f*(sizeRct + sizeGap), (2 * sizeRct + 3 * sizeGap) / ratio, sizeGap, colBrd); //Bottom
	GRAPHICS_X::DRAW_RECT(posX - 1.0f*(sizeRct + sizeGap) / ratio, posY, sizeGap / ratio, sizeRct, colBrd); //Left
	GRAPHICS_X::DRAW_RECT(posX + 0.0f*(sizeRct + sizeGap) / ratio, posY, sizeGap / ratio, sizeRct, colBrd); //Middle
	GRAPHICS_X::DRAW_RECT(posX + 1.0f*(sizeRct + sizeGap) / ratio, posY, sizeGap / ratio, sizeRct, colBrd); //Right

	float posRctX, posDotX, posDotY;

	//Draw left Stick
	posRctX = posX    - 0.5f * (sizeGap + sizeRct) / ratio;
	posDotX = posRctX + 0.5f * lx * (sizeRct - sizeDot) / ratio; //0 is left 1 is right
	posDotY = posY    - 0.5f * ly * (sizeRct - sizeDot); //0 is top 1 is bottom

	GRAPHICS_X::DRAW_RECT(posRctX, posY,    sizeRct / ratio, sizeRct, colRct); //Rectangle
	GRAPHICS_X::DRAW_RECT(posDotX, posDotY, sizeDot / ratio, sizeDot, colDot); //Dot

																	 //Draw right Stick
	posRctX = posX    + 0.5f * (sizeGap + sizeRct) / ratio;
	posDotX = posRctX + 0.5f * rx * (sizeRct - sizeDot) / ratio; //0 is left 1 is right
	posDotY = posY    - 0.5f * ry * (sizeRct - sizeDot); //0 is top 1 is bottom

	GRAPHICS_X::DRAW_RECT(posRctX, posY,    sizeRct / ratio, sizeRct, colRct); //Rectangle
	GRAPHICS_X::DRAW_RECT(posDotX, posDotY, sizeDot / ratio, sizeDot, colDot); //Dot
}
