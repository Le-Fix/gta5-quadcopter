#include "ColorRGBA.h"

//Constructors

ColorRGBA::ColorRGBA() : r(0), g(0), b(0), a(255) {}

ColorRGBA::ColorRGBA(int _r, int _g, int _b, int _a) : r(_r), g(_g), b(_b), a(_a) {}

ColorRGBA::~ColorRGBA() {}

//Standard colours

ColorRGBA ColorRGBA::black()
{
	return ColorRGBA(0, 0, 0);
}
ColorRGBA ColorRGBA::white()
{
	return ColorRGBA(255, 255, 255);
}
ColorRGBA ColorRGBA::red()
{
	return ColorRGBA(255, 0, 0);
}
ColorRGBA ColorRGBA::green()
{
	return ColorRGBA(0, 255, 0);
}
ColorRGBA ColorRGBA::blue()
{
	return ColorRGBA(0, 0, 255);
}
ColorRGBA ColorRGBA::transparent()
{
	return ColorRGBA(0, 0, 0, 0);
}