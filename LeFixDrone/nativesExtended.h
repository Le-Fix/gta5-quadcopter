#pragma once

#define _USE_MATH_DEFINES

#include "inc\types.h"
#include "inc\natives.h"
#include "inc\enums.h"

#include <Eigen/Dense>
#include <string>
#include <cmath>

#include "Graphics\ColorRGBA.h"

using namespace Eigen;

std::string toString(const Vector3f &v);
std::string toString(const Quaternionf &q);

Vector3f Convert(const Vector3 &v);

float getHeading(const Vector3f &v);

float RadToDeg(const float &rad);
float DegToRad(const float &deg);

namespace ENTITY_X
{
	Quaternionf GET_ENTITY_QUATERNION(Entity e);
	void SET_ENTITY_QUATERNION(Entity e, Quaternionf q);
	Vector3f GET_ENTITY_COORDS(Entity e);
	Vector3f GET_ENTITY_VELOCITY(Entity e);
	void SET_ENTITY_COORDS(Entity e, Vector3f);
	void SET_ENTITY_VELOCITY(Entity e, Vector3f vel);
	void APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(Entity entity, Vector3f force, bool isDirRel, bool isStrong, bool isMassRel);
	void APPLY_FORCE_TO_ENTITY(Entity entity, Vector3f force, Vector3f offset, int boneIndex, bool isDirRel, bool isStrong, bool isMassRel);
	void APPLY_MOMENTUM_TO_ENTITY(Entity entity, Vector3f momentum, int boneIndex, bool isStrong, bool isMassRel);
}

namespace CAM_X
{
	void POINT_CAM_AT_COORD(Camera c, Vector3f pos);
	void SET_CAM_QUATERNION(Camera c, Quaternionf q);
	Quaternionf GET_CAM_QUATERNION(Camera c);
	void SET_CAM_COORD(Camera c, Vector3f pos);
	Vector3f GET_CAM_COORD(Camera c);
	void ATTACH_CAM_TO_ENTITY(Camera c, Entity e, Vector3f pos, bool isRel);
}

namespace GRAPHICS_X
{
	void DRAW_RECT(float posX, float posY, float width, float height, ColorRGBA col);
	void DRAW_DOT(float posX, float posY, float size, ColorRGBA col);
}

namespace UI_X
{
	void DRAW_TEXT(float posX, float posY, float scale, int font, ColorRGBA col, std::string text);
	void SET_TEXT_COLOUR(ColorRGBA col);
	void SET_TEXT_DROPSHADOW(int distance, ColorRGBA col);
	void SET_TEXT_EDGE(Hash p0, ColorRGBA col);
}