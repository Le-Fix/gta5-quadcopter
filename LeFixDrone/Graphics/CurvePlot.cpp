#include "CurvePlot.h"

CurvePlot::CurvePlot(Gamepad* _pad) : pad(_pad)
{
	for (int n = 0; n < NUM_POINTS; n++)
	{
		val[ePi][n] = -1.0f + (n + 0.0f / 3.0f) * 2.0f / NUM_POINTS;
		val[eRi][n] = -1.0f + (n + 1.0f / 3.0f) * 2.0f / NUM_POINTS;
		val[eYi][n] = -1.0f + (n + 2.0f / 3.0f) * 2.0f / NUM_POINTS;
	}
}


CurvePlot::~CurvePlot()
{
}

void CurvePlot::refreshData()
{
	//Calculate dot values
	for (int n = 0; n < NUM_POINTS; n++)
	{
		val[ePv][n]	= Steuerung::calculateDegSecPitch(val[ePi][n]);
		val[eRv][n]	= Steuerung::calculateDegSecRoll( val[eRi][n]);
		val[eYv][n]	= Steuerung::calculateDegSecYaw(  val[eYi][n]);
	}

	//Max and critical
	float maxP = Steuerung::calculateDegSecPitch(1.0f);
	float maxR = Steuerung::calculateDegSecRoll(1.0f);
	float maxY = Steuerung::calculateDegSecYaw(1.0f); 

	max = 0.0f;
	if (maxP > max) max = maxP;
	if (maxR > max) max = maxR;
	if (maxY > max) max = maxY;

	critical = max > 1200.0f;

	maxPitch = (int)maxP;
	maxRoll  = (int)maxR;
	maxYaw   = (int)maxY;
}

void CurvePlot::draw()
{
	static ColorRGBA colDot[3] = { ColorRGBA(0, 255, 0, 200), ColorRGBA(255, 0, 0, 200), ColorRGBA(0, 0, 255, 200) };
	static ColorRGBA colRct = ColorRGBA(0, 0, 0, 80);
	static ColorRGBA colBrd = ColorRGBA(0, 0, 0, 120);

	static float sizeDot = 0.004f;//Size of Dots
	static float sizeRct = 0.7f;  //Size of Graph Retangle
	static float sizeBrd = 0.01f; //Border Thickness

	static float posX = 0.5f; //X Position of Center
	static float posY = 0.4f; //Y Position of Center

	static int font = 0; //Standard Font (like Arial)
	static float text_scale = 0.8f * sizeRct; //Size

	float ratio = GRAPHICS::_GET_ASPECT_RATIO(FALSE);

	//Draw background
	GRAPHICS_X::DRAW_RECT(posX, posY - 0.5f*(sizeRct + sizeBrd), (sizeRct + 2 * sizeBrd) / ratio, sizeBrd, colBrd); //Top
	GRAPHICS_X::DRAW_RECT(posX, posY + 0.5f*(sizeRct + sizeBrd), (sizeRct + 2 * sizeBrd) / ratio, sizeBrd, colBrd); //Bottom
	GRAPHICS_X::DRAW_RECT(posX - 0.5f*(sizeRct + sizeBrd) / ratio, posY, sizeBrd / ratio, sizeRct, colBrd); //Left
	GRAPHICS_X::DRAW_RECT(posX + 0.5f*(sizeRct + sizeBrd) / ratio, posY, sizeBrd / ratio, sizeRct, colBrd); //Right

	GRAPHICS_X::DRAW_RECT(posX, posY, sizeRct / ratio, sizeRct, colRct);

	//Draw Text Pitch
	float textPosX1, textPosX2, textPosY;
	textPosX1 = posX + (sizeBrd - 0.5f*sizeRct) / ratio;
	textPosX2 = textPosX1 + text_scale * 0.24f / ratio;
	textPosY = posY + (sizeBrd - 0.5f*sizeRct);
	UI_X::DRAW_TEXT(textPosX1, textPosY, text_scale, font, colDot[ePv], "PITCH");
	UI_X::DRAW_TEXT(textPosX2, textPosY, text_scale, font, colDot[ePv], std::to_string(maxPitch)); textPosY += text_scale * 0.06f;
	UI_X::DRAW_TEXT(textPosX1, textPosY, text_scale, font, colDot[eRv], "ROLL" );
	UI_X::DRAW_TEXT(textPosX2, textPosY, text_scale, font, colDot[eRv], std::to_string(maxRoll )); textPosY += text_scale * 0.06f;
	UI_X::DRAW_TEXT(textPosX1, textPosY, text_scale, font, colDot[eYv], "YAW"  );
	UI_X::DRAW_TEXT(textPosX2, textPosY, text_scale, font, colDot[eYv], std::to_string(maxYaw  ));

	//Draw Points
	for (int n = 0; n < NUM_POINTS; n++)
	{
		GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * val[ePi][n] / ratio, posY - 0.5f * sizeRct * val[ePv][n] / max, sizeDot, colDot[ePv]);
		GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * val[eRi][n] / ratio, posY - 0.5f * sizeRct * val[eRv][n] / max, sizeDot, colDot[eRv]);
		GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * val[eYi][n] / ratio, posY - 0.5f * sizeRct * val[eYv][n] / max, sizeDot, colDot[eYv]);
	}

	//Current values
	current[ePi] = -pad->stick_right_y;
	current[eRi] =  pad->stick_right_x;
	current[eYi] = -pad->stick_left_x;

	current[ePv] = Steuerung::calculateDegSecPitch(current[ePi]);
	current[eRv] = Steuerung::calculateDegSecRoll(current[eRi]);
	current[eYv] = Steuerung::calculateDegSecYaw(current[eYi]);

	GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * current[ePi] / ratio, posY - 0.5f * sizeRct * current[ePv] / max, sizeDot*3, colDot[ePv]);
	GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * current[eRi] / ratio, posY - 0.5f * sizeRct * current[eRv] / max, sizeDot*3, colDot[eRv]);
	GRAPHICS_X::DRAW_DOT(posX + 0.5f * sizeRct * current[eYi] / ratio, posY - 0.5f * sizeRct * current[eYv] / max, sizeDot*3, colDot[eYv]);
}