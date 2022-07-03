
#include "Collision.h"


Collision::Collision(vec3 center, float width, float height, float depth)
{
	this->localCenter = center;
	this->localWidth = width;
	this->localHeight = height;
	this->localDepth = depth;	

	this->isTrigger = false;
	this->active = true;
}


Collision::~Collision() 
{
	
}


void Collision::UpdateCollsion(vec3 worldPos, vec3 rot, vec3 scale) 
{
	if (scale != this->scale) this->scale = scale;
	objCenter = localCenter + worldPos;
	
	width = localWidth * scale.x;
	height = localHeight * scale.y;
	depth = localDepth * scale.z;

	CalPoints();
	//printf("(%f, %f, %f, %f, %f, %f)\n", minX, maxX, minY, maxY, minZ, maxZ);
}										 
										 
										 
void Collision::CalPoints() 			 
{										 
	float halfW = width / 2;
	float halfH = height / 2;
	float halfD = depth / 2;
	minX = objCenter.x - halfW;
	maxX = objCenter.x + halfW;
	minY = objCenter.y - halfH;
	maxY = objCenter.y + halfH;
	minZ = objCenter.z - halfD;
	maxZ = objCenter.z + halfD;
}


bool Collision::IsCollide(Collision* other) 
{
	if (!active) return false;

	bool isXCollide = isLineOverlap(this->minX, this->maxX, other->minX, other->maxX);
	bool isYCollide = isLineOverlap(this->minY, this->maxY, other->minY, other->maxY);
	bool isZCollide = isLineOverlap(this->minZ, this->maxZ, other->minZ, other->maxZ);

	return isXCollide && isYCollide && isZCollide;
}


bool Collision::isLineOverlap(float Amin, float Amax, float Bmin, float Bmax) 
{
	float minMax = 0; //尾端較小線段的尾端
	float maxMin = 0; //尾端較大線段的頭端

	if (Amax < Bmax) { 
		//a物體在b物體左側 
		minMax = Amax; 
		maxMin = Bmin; 
	} 	
	else { 
		//a物體在b物體右側 
		minMax = Bmax; 
		maxMin = Amin; 
	} 
	return (minMax > maxMin);
}