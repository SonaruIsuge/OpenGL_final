
#include "Transform.h"


Transform::Transform()
{
	position = vec3();
	rotation = vec3();
	scale = vec3();
	parent = nullptr;
}


Transform::~Transform() 
{
	if (parent != nullptr) delete parent;
}


void Transform::ResetTransform() 
{
	position = vec3();
	rotation = vec3();
	scale = vec3();
}


mat4 Transform::GetTRSMatrix() 
{
	mat4 trsMat;

	if (parent != nullptr) trsMat = parent->GetTRSMatrix();

	// wrong rotation 
	// TODO: fix rotate 
	trsMat = trsMat * Translate(position) * RotateX(rotation.x) * RotateY(rotation.y) * RotateZ(rotation.z) * Scale(scale);
	return trsMat;
}


vec3 Transform::GetWorldPosition() 
{
	vec3 worldPos = vec3();

	if (parent != nullptr) worldPos = parent->GetWorldPosition();

	worldPos = worldPos + position;

	return worldPos;
}