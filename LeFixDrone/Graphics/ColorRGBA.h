#pragma once
class ColorRGBA
{
public:
	ColorRGBA();
	ColorRGBA(int _r, int _g, int _b, int _a = 255);
	~ColorRGBA();

	static ColorRGBA black();
	static ColorRGBA white();
	static ColorRGBA red();
	static ColorRGBA green();
	static ColorRGBA blue();
	static ColorRGBA transparent();

	int r, g, b, a;
};

