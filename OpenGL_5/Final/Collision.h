#pragma once

#include "../Header/Angel.h"


class Collision	
{
private:

	vec3 localCenter;
	float localWidth;
	float localDepth;
	float localHeight;
	bool isTrigger;
	bool active;

	void CalPoints();
	bool isLineOverlap(float Amin, float Amax, float Bmin, float Bmax);

public:

	// for world coodination
	vec3 objCenter;
	vec3 scale;
	float width;	// X方向
	float depth;	// Z方向
	float height;	// Y方向

	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	Collision(vec3 center, float width, float height, float depth);
	virtual ~Collision();

	void UpdateCollsion(vec3 worldPos, vec3 rot, vec3 scale);
	bool IsCollide(Collision* other);

	void SetTrigger(bool trigger) { isTrigger = trigger; }
	bool IsTirgger() { return isTrigger; }
	void SetActive(bool active) { this->active = active; }
	bool IsActive() { return active; }
};