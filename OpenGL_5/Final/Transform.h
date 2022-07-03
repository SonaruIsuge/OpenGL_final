#pragma once
#include "../Header/Angel.h"

class Transform 
{
private:
	


public:

	vec3 position;
	vec3 rotation;
	vec3 scale;
	Transform* parent;	

	Transform();
	virtual ~Transform();
	void ResetTransform();
	mat4 GetTRSMatrix();
	vec3 GetWorldPosition();
};