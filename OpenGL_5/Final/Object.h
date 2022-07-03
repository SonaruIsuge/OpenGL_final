#pragma once

#include "Transform.h"
#include "../Common/CShape.h"
#include "Collision.h"


class Object 
{
protected:


private:


public:
	GLuint diffuseMap;
	GLuint lightMap;
	GLuint normalMap;

	Transform* transform;
	CShape* shape;
	Collision* collision;

	Object(CShape* shape);
	virtual ~Object();

	void Init(vec3 pos, vec3 rot, vec3 scale);
	void SetTexShaderMode(TextureMode texMode = TextureMode::NO_TEXTURE, ShadingMode shadingMode = ShadingMode::FLAT_SHADING_CPU);
	void SetDiffuseMap(char dMap[40]);
	void SetLightMap(char lMap[40]);
	void SetNormalMap(char nMap[40]);
	void Draw();
	void Update(float delta, LightSource light);
	void Update(float delta);
	void Update(float dt, const LightSource& Light1, const LightSource& Light2);
	void Destroy();

	void InitCollision(Collision* collision);
};