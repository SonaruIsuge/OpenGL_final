#pragma once

#include <vector>
#include "../Header/Angel.h"
using namespace std;


#define SAFE_DELETE(p)  do { delete (p); (p) = nullptr; } while(0)

typedef struct ReadShapeData
{
	int vtxNum;
	vector<vec4> points;
	vector<vec2> texs;
	vector<vec3> normals;

} ShapeData;


class ObjLoader 
{
	
protected:
	vector<vec4> tempPoints;
	vector<vec3> tempNormals;
	vector<vec2> tempTexs;

	ObjLoader();
	~ObjLoader();
	
	static ObjLoader* _instance;

public:
	vector<vec4> points;
	vector<vec3> normals;
	vector<vec2> texs;

	static ObjLoader* Create();
	static ObjLoader* Instance();
	void Destroy();

	ShapeData GetObjData(char path[]);
	void ClearData();
};