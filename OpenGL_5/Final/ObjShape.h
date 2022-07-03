#pragma once
#include "../header/Angel.h"
#include "../Common/CShape.h"
#include "ObjLoader.h"


class ObjShape : public CShape 
{
private:


public:
	ObjShape(ShapeData data);
	~ObjShape() {};

	void update(float dt, point4 vLightPos, color4 vLightI);
	void update(float dt, const LightSource& Lights);
	void update(float dt); // 不計算光源的照明
	void update(float dt, const LightSource& Light1, const LightSource& Light2);

	void draw();
	void drawW();
};