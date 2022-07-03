
#include "Object.h"
#include "../Common/CTexturePool.h"

Object::Object(CShape* shape) 
{
	this->shape = shape;
	this->transform = new Transform;
	//預設沒有collision
	collision = nullptr;
}


Object::~Object() {}


void Object::SetTexShaderMode(TextureMode texMode, ShadingMode shadingMode) 
{
	this->shape->setTextureLayer(texMode);
	this->shape->setShadingMode(shadingMode);
}


void Object::Init(vec3 pos, vec3 rot, vec3 scale)
{
	this->transform->position = pos;
	this->transform->rotation = rot;
	this->transform->scale = scale;

	this->shape->setShader();
}


void Object::SetDiffuseMap(char dMap[40]) 
{
	auto texturepool = CTexturePool::create();
	diffuseMap = texturepool->addTexture(dMap);
}


void Object::SetLightMap(char lMap[40]) 
{
	auto texturepool = CTexturePool::create();
	lightMap = texturepool->addTexture(lMap);
}


void Object::SetNormalMap(char nMap[40]) 
{
	auto texturepool = CTexturePool::create();
	normalMap = texturepool->addTexture(nMap);
}


void Object::Draw() 
{
	if (shape->getTextureMode() >= TextureMode::DIFFUSE_MAP) 
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
	}
	if (shape->getTextureMode() >= TextureMode::LIGHT_MAP)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lightMap);
	}
	if(shape->getTextureMode() >= TextureMode::NORMAL_MAP)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalMap);
	}
	this->shape->draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Object::Update(float delta, LightSource light) 
{
	this->shape->setTRSMatrix(this->transform->GetTRSMatrix());
	this->shape->update(delta, light);
	if (collision != nullptr) collision->UpdateCollsion(transform->position, transform->rotation, transform->scale);
}


void Object::Update(float delta) 
{
	this->shape->setTRSMatrix(this->transform->GetTRSMatrix());
	this->shape->update(delta);
	if(collision != nullptr) collision->UpdateCollsion(transform->position, transform->rotation, transform->scale);
}


void Object::Update(float delta, const LightSource& Light1, const LightSource& Light2) 
{
	this->shape->setTRSMatrix(this->transform->GetTRSMatrix());
	this->shape->update(delta, Light1, Light2);
	if (collision != nullptr) collision->UpdateCollsion(transform->position, transform->rotation, transform->scale);
}


void Object::Destroy()
{
	delete this->shape;
	delete this->transform;
	if (collision != nullptr) delete this->collision;
}


void Object::InitCollision(Collision* collision) 
{

	this->collision = collision;
}