#include "menu.h"

#include "..\nativesExtended.h"
#include "..\Graphics\ColorRGBA.h"

void draw_caption(std::string caption, float lineWidth)
{
	draw_menu_line(caption, "", -1, lineWidth, false);
}
void draw_data_line(menuValue& data, int index, float lineWidth, bool active)
{
	std::string value = data.to_string();
	std::string name = data.getName();

	draw_menu_line(name, value, index, lineWidth, active);
}
void draw_menu_line(std::string name, std::string value, int index, float lineWidth, bool active)
{
	//Screen values
	int screenWidth, screenHeight;
	GRAPHICS::_GET_ACTIVE_SCREEN_RESOLUTION(&screenWidth, &screenHeight);

	// default values
	ColorRGBA text_col = ColorRGBA(245, 245, 246, 255);
	ColorRGBA rect_col = ColorRGBA( 80,  80,  80, 235);

	float lineLeft = 5.0f;
	float textLeft = 5.0f + lineLeft;
	float text_scale = 0.35f;
	int font = 0;
	float lineHeight = text_scale * 85.0f;
	float lineTop = 60.0f + (lineHeight*1.1f) * index;

	// correcting values for active line
	if (active)
	{
		text_col = ColorRGBA::black();
		rect_col = ColorRGBA(255, 195, 115, 255);
		text_scale = 0.37f;
	}
	if (index == -1)
	{
		text_col = ColorRGBA::white();
		rect_col = ColorRGBA::black();
		text_scale = 0.70f;
		font = 1;
		textLeft += 5.0f;
		lineHeight = text_scale * 65.0f;
		lineTop = 12.0f;
	}

	//correcting text size for different displays
	text_scale = text_scale * (900.0f / (float)screenHeight);

	float lineWidthScaled = lineWidth / (float)screenWidth; // line width
	float lineLeftScaled = lineLeft / (float)screenWidth; // line left offset
	float lineHeightScaled = lineHeight / (float)screenHeight; // line height
	float lineTopScaled = lineTop / (float)screenHeight; // line top offset

	float textLeftScaled = textLeft / (float)screenWidth; // text left offset
	float textLeftScaled2 = (textLeft + lineWidth - 75.0f) / (float)screenWidth; // value left offset

	UI_X::DRAW_TEXT(textLeftScaled,  lineTopScaled + 0.06f * lineHeightScaled, text_scale, font, text_col, name);
	UI_X::DRAW_TEXT(textLeftScaled2, lineTopScaled + 0.06f * lineHeightScaled, text_scale, font, text_col, value);

	// rect
	GRAPHICS_X::DRAW_RECT(
		lineLeftScaled + lineWidthScaled * 0.5f,
		lineTopScaled + lineHeightScaled * 0.5f,
		lineWidthScaled, lineHeightScaled,
		rect_col
	);

}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}