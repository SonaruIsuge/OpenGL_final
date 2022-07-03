
#include "Application.h"
#include "InputManager.h"
#include "../header/Angel.h"
#include "../Common/CQuad.h"
#include "../Common/CSolidCube.h"
#include "../Common/CSolidSphere.h"
#include "../Common/CWireSphere.h"
#include "../Common/CWireCube.h"
#include "../Common/CChecker.h"
#include "../Common/CCamera.h"
#include "../Common/CTexturePool.h"
#include "../Common/CShaderPool.h"
#include "../png_loader.h"
#include "Object.h"
#include "ObjLoader.h"
#include "ObjShape.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number


// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;

// For Objects
Object* Floor;
Object* walls[4];
Object* innerWalls[6];

Object* Cube;
Object* Ame;
Object* VikingRoom;
Object* Torii;
Object* Shrine;

Object* lightObj;

// For View Point
GLfloat g_fRadius = 10.0;
GLfloat g_fTheta = 45.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;

//----------------------------------------------------------------------------
// Part 2 : for single light source
bool g_bAutoRotating = false;
float g_fElapsedTime = 0;
float g_fLightRadius = 12;
float g_fLightTheta = 0;

float g_fLightR = 1.0f;
float g_fLightG = 0.0f;
float g_fLightB = 0.0f;

LightSource g_Light1 = {
	color4(1.0f, 1.0f, 1.0f, 1.0f), // ambient 
	color4(1.0f, 1.0f, 1.0f, 1.0f), // diffuse
	color4(1.0f, 1.0f, 1.0f, 1.0f), // specular
	point4(12.0f, 12.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  // spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0,	// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
	LightType::OMNI_LIGHT
};

LightSource g_Light2 = {
	color4(1.0f, 1.0f, 1.0f, 1.0f), // ambient 
	color4(1.0f, 1.0f, 1.0f, 1.0f), // diffuse
	color4(1.0f, 1.0f, 1.0f, 1.0f), // specular
	point4(-10.0f, 10.0f, -10.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  // spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0,	// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
	LightType::OMNI_LIGHT
};

//----------------------------------------------------------------------------

// Texture 測試用

int g_iTexWidth, g_iTexHeight;


// 函式原型宣告
void SetRoom();
bool DetectCollision();
void DetectCollider();
void UpdateLightColor(float delta);
//void SetUI();

Application::Application(){}
Application::~Application(){}


// ---------------------------------------------
// main function
void Application::Start() 
{
	//point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4 eye(15, 3, 15, 1);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);

	//auto texturepool = CTexturePool::create();

	vec3 pos, rot, scale;
	
	SetRoom();

	Cube = new Object(new CSolidCube);
	pos = vec3(10.0f, 1.5f, 10.0f); rot = vec3(0); scale = vec3(3);
	Cube->shape->setMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Cube->shape->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	Cube->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	Cube->SetDiffuseMap("texture/Masonry.Brick.png");
	Cube->SetNormalMap("texture/Masonry.Brick.normal.png");
	Cube->Init(pos, rot, scale);
	Cube->InitCollision(new Collision(vec3(0), 1, 1, 1));

	// Object from .obj
	auto objLoader = ObjLoader::Instance();

	Ame = new Object(new ObjShape(objLoader->GetObjData("Final/Ame.obj")));
	pos = vec3(-10.0f, 0.01f, -10.0f); rot = vec3(0); scale = vec3(0.01);
	Ame->SetTexShaderMode(TextureMode::DIFFUSE_MAP, ShadingMode::PHONG_SHADING);
	Ame->SetDiffuseMap("texture/Ame_diffuse.png");
	Ame->Init(pos, rot, scale);
	
	objLoader->ClearData();

	VikingRoom = new Object(new ObjShape(objLoader->GetObjData("Final/room.obj")));
	pos = vec3(10.0f, 0.01f, -10.0f); rot = vec3(0, -90, 0); scale = vec3(73);
	VikingRoom->SetTexShaderMode(TextureMode::DIFFUSE_MAP, ShadingMode::PHONG_SHADING);
	VikingRoom->SetDiffuseMap("texture/room_diffuse.png");
	VikingRoom->Init(pos, rot, scale);

	objLoader->ClearData();

	Torii = new Object(new ObjShape(objLoader->GetObjData("Final/Torii.obj")));
	pos = vec3(-6.0f, 5.01f, 10.0f); rot = vec3(0, -90, 0); scale = vec3(8);
	Torii->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	Torii->SetDiffuseMap("texture/Torii_diffuse.png");
	Torii->SetNormalMap("texture/Torii_normal.png");
	Torii->Init(pos, rot, scale);
	Torii->InitCollision(new Collision(vec3(0.0f, 1.25f, 0.0f), 0.125, 0.625, 0.5));
	Torii->collision->SetTrigger(true);

	objLoader->ClearData();
	Shrine = new Object(new ObjShape(objLoader->GetObjData("Final/Shrine.obj")));
	pos = vec3(-15.0f, 0.01f, 10.0f); rot = vec3(0, -90, 0); scale = vec3(0.015);
	Shrine->shape->setMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Shrine->shape->setKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	Shrine->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	Shrine->SetDiffuseMap("texture/Shrine_diffuse.png");
	Shrine->SetNormalMap("texture/Shrine_normal.png");
	Shrine->Init(pos, rot, scale);


	// 設定 代表 Light 的 WireSphered
	lightObj = new Object(new CWireSphere(0.25f, 6, 3));
	pos = vec3(g_Light1.position.x, g_Light1.position.y, g_Light1.position.z);
	rot = vec3(0); scale = vec3(1);
	lightObj->shape->setColor(g_Light1.diffuse);
	lightObj->shape->setLightingDisable();
	lightObj->SetTexShaderMode(TextureMode::NO_TEXTURE, ShadingMode::GOURAUD_SHADING_GPU);
	lightObj->Init(pos, rot, scale);

	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	Floor->shape->setProjectionMatrix(mpx);
	for(auto wall : walls) wall->shape->setProjectionMatrix(mpx);
	for (auto iWall : innerWalls) iWall->shape->setProjectionMatrix(mpx); 

	Cube->shape->setProjectionMatrix(mpx);
	Ame->shape->setProjectionMatrix(mpx);
	VikingRoom->shape->setProjectionMatrix(mpx);
	Torii->shape->setProjectionMatrix(mpx);
	Shrine->shape->setProjectionMatrix(mpx);
	lightObj->shape->setProjectionMatrix(mpx);
}


void Application::Draw() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Floor->Draw();
	for(auto wall : walls) wall->Draw();
	//for (auto iWall : innerWalls) iWall->Draw();
	innerWalls[0]->Draw();
	innerWalls[2]->Draw();
	innerWalls[3]->Draw();
	innerWalls[5]->Draw();

	Cube->Draw();
	Ame->Draw();
	VikingRoom->Draw();
	Torii->Draw();
	Shrine->Draw();
	lightObj->Draw();

	glDepthMask(GL_FALSE);

	innerWalls[1]->Draw();
	innerWalls[4]->Draw();

	glDisable(GL_BLEND);	// 關閉 Blending
	glDepthMask(GL_TRUE);	// 開啟對 Z-Buffer 的寫入操作
}


void updateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime * (float)M_PI_2;
	if (g_fLightTheta >= (float)M_PI * 2.0f) {
		g_fLightTheta -= (float)M_PI * 2.0f;
		g_fElapsedTime -= 4.0f;
	}
	g_Light1.position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light1.position.z = g_fLightRadius * sinf(g_fLightTheta);
	lightObj->transform->position = vec3(g_Light1.position.x, lightObj->transform->position.y, g_Light1.position.z);
}


void Application::Update(float delta) 
{

	if (g_bAutoRotating) { // Part 2 : 重新計算 Light 的位置
		updateLightPosition(delta);
		UpdateLightColor(delta);
	}
	else {
		g_Light1.diffuse = color4(1.0f);
		lightObj->shape->setColor(g_Light1.diffuse);
	}

	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	camera->UpdateLookByMouse();
	camera->Move(delta, DetectCollision());
	DetectCollider();

	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		Floor->shape->setViewMatrix(mvx);
		for (auto wall : walls) wall->shape->setViewMatrix(mvx);
		for (auto iWall : innerWalls) iWall->shape->setViewMatrix(mvx);
		
		Cube->shape->setViewMatrix(mvx);
		Ame->shape->setViewMatrix(mvx);
		VikingRoom->shape->setViewMatrix(mvx);
		Torii->shape->setViewMatrix(mvx);
		Shrine->shape->setViewMatrix(mvx);
		lightObj->shape->setViewMatrix(mvx);
	}

	// 如果需要重新計算時，在這邊計算每一個物件的顏色
	Floor->Update(delta, g_Light1);
	for (auto wall : walls) wall->Update(delta, g_Light1);
	for (auto iWall : innerWalls) iWall->Update(delta, g_Light1);
	
	Cube->Update(delta, g_Light1);
	Ame->Update(delta, g_Light1);
	VikingRoom->Update(delta, g_Light1);
	Torii->Update(delta, g_Light1);
	printf("(%f, %f, %f, %f, %f, %f)\n", Torii->collision->minX, Torii->collision->maxX, Torii->collision->minY, Torii->collision->maxY, Torii->collision->minZ, Torii->collision->maxZ);
	Shrine->Update(delta, g_Light1);
	lightObj->Update(delta);
}



void Application::Destroy()
{
	Floor->Destroy();
	walls[0]->Destroy();
	walls[1]->Destroy();
	walls[2]->Destroy();
	walls[3]->Destroy();
	innerWalls[0]->Destroy();
	innerWalls[1]->Destroy();
	innerWalls[2]->Destroy();
	innerWalls[3]->Destroy();
	innerWalls[4]->Destroy();
	innerWalls[5]->Destroy();

	Cube->Destroy();
	Ame->Destroy();
	VikingRoom->Destroy();
	Torii->Destroy();
	Shrine->Destroy();
	lightObj->Destroy();
	CCamera::getInstance()->destroyInstance();
	CTexturePool::getInstance()->destroyInstance();
	CShaderPool::getInstance()->destroyInstance();
	InputManager::getInstance()->destroy();
}


// ---------------------------------------------
//handle input 
void Application::OnKeyboardHit(unsigned char key, int x, int y) 
{
	InputManager::getInstance()->onKeyPressed(key);

	if (InputManager::getInstance()->isKeyClick(SPACE_KEY)) 
	{
		//g_bAutoRotating = !g_bAutoRotating;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_R)) 
	{
		if (g_fLightR <= 0.95f) g_fLightR += 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_r)) 
	{
		if (g_fLightR >= 0.05f) g_fLightR -= 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_G)) 
	{
		if (g_fLightG <= 0.95f) g_fLightG += 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_g)) 
	{
		if (g_fLightG >= 0.05f) g_fLightG -= 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_B)) 
	{
		if (g_fLightB <= 0.95f) g_fLightB += 0.05f;
	}

	if (InputManager::getInstance()->isKeyClick(KEYCODE_b)) 
	{
		if (g_fLightB >= 0.05f) g_fLightB -= 0.05f;
	}

	g_Light1.diffuse = color4(g_fLightR, g_fLightG, g_fLightB, 1.0f);
	lightObj->shape->setColor(g_Light1.diffuse);

	if (InputManager::getInstance()->isKeyClick(ESCAPE_KEY)) 
	{
		glutIdleFunc(NULL);
		Destroy();
		exit(EXIT_SUCCESS);
	}
}


void Application::OnKeyboardRelease(unsigned char key, int x, int y) 
{
	InputManager::getInstance()->ReleaseKey(key);
}



void Application::OnMouseClick(int button, int state, int x, int y) 
{
	InputManager::getInstance()->onMouseClick(button, state);
}


void Application::OnMouseMoveWithoutClick(int x, int y) 
{
	InputManager::getInstance()->onMouseMove(x, y);
}


void Application::OnMouseMoveWithClick(int x, int y)
{
	InputManager::getInstance()->onMouseMove(x, y);
}


// 函式實作
void SetRoom() 
{
	vec3 pos, rot, scale;

	Floor = new Object(new CQuad);
	pos = vec3(0); rot = vec3(0); scale = vec3(40, 1, 40);
	Floor->shape->setMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Floor->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	Floor->shape->setTiling(10, 10);
	Floor->SetDiffuseMap("texture/floor_diffuse.png");
	//Floor->SetNormalMap("texture/floor_normal.png");
	Floor->SetLightMap("texture/lightMap1.png");
	Floor->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::LIGHT_MAP, ShadingMode::PHONG_SHADING);
	Floor->Init(pos, rot, scale);

	// Left Wall
	pos = vec3(-20, 5.01, 0);	rot = vec3(0, 0, -90);	scale = vec3(10, 1, 40);
	walls[0] = new Object(new CQuad);
	walls[0]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	walls[0]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	walls[0]->SetDiffuseMap("texture/wall_diffuse.png");
	walls[0]->SetNormalMap("texture/wall_normal.png");
	walls[0]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	walls[0]->Init(pos, rot, scale);
	// Right Wall
	pos = vec3(20, 5.01, 0);	rot = vec3(0, 0, 90);	scale = vec3(10, 1, 40);
	walls[1] = new Object(new CQuad);
	walls[1]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	walls[1]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	walls[1]->SetDiffuseMap("texture/wall_diffuse.png");
	walls[1]->SetNormalMap("texture/wall_normal.png");
	walls[1]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	walls[1]->Init(pos, rot, scale);
	// Front Wall
	pos = vec3(0, 5.01, 20);	rot = vec3(-90, 90, 0);	scale = vec3(10, 1, 40);
	walls[2] = new Object(new CQuad);
	walls[2]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	walls[2]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	walls[2]->SetDiffuseMap("texture/wall_diffuse.png");
	walls[2]->SetNormalMap("texture/wall_normal.png");
	walls[2]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	walls[2]->Init(pos, rot, scale);
	// Back Wall
	pos = vec3(0, 5.01, -20);	rot = vec3(90, 90, 0);	scale = vec3(10, 1, 40);
	walls[3] = new Object(new CQuad);
	walls[3]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	walls[3]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	walls[3]->SetDiffuseMap("texture/wall_diffuse.png");
	walls[3]->SetNormalMap("texture/wall_normal.png");
	walls[3]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	walls[3]->Init(pos, rot, scale);

	//Inner wall
	// horizontal Left
	pos = vec3(-16.01, 5.01, 0);	rot = vec3(0, 0, 0);	scale = vec3(8, 10, 1);
	innerWalls[0] = new Object(new CSolidCube);
	innerWalls[0]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[0]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[0]->SetDiffuseMap("texture/tile_diffuse.png");
	innerWalls[0]->SetNormalMap("texture/tile_normal.png");
	innerWalls[0]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[0]->Init(pos, rot, scale);
	innerWalls[0]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

	// horizontal middle
	pos = vec3(0, 5.01, 0);	rot = vec3(0, 0, 0);	scale = vec3(16, 10, 1);
	innerWalls[1] = new Object(new CSolidCube);
	innerWalls[1]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 0.4f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[1]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[1]->SetDiffuseMap("texture/glass_diffuse.png");
	innerWalls[1]->SetNormalMap("texture/glass_normal.png");
	innerWalls[1]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[1]->Init(pos, rot, scale);
	innerWalls[1]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

	//horizontal right
	pos = vec3(16.01, 5.01, 0);	rot = vec3(0, 0, 0);	scale = vec3(8, 10, 1);
	innerWalls[2] = new Object(new CSolidCube);
	innerWalls[2]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[2]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[2]->SetDiffuseMap("texture/tile_diffuse.png"); 
	innerWalls[2]->SetNormalMap("texture/tile_normal.png");
	innerWalls[2]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[2]->Init(pos, rot, scale);
	innerWalls[2]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

	//verticle forward
	pos = vec3(0.01, 5.01, -16.01);	rot = vec3(0, 0, 0);	scale = vec3(1, 10, 8);
	innerWalls[3] = new Object(new CSolidCube);
	innerWalls[3]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[3]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[3]->SetDiffuseMap("texture/tile_diffuse.png");
	innerWalls[3]->SetNormalMap("texture/tile_normal.png");
	innerWalls[3]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[3]->Init(pos, rot, scale);
	innerWalls[3]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

	//verticle middle
	pos = vec3(0, 5.01, 0);	rot = vec3(0);	scale = vec3(1, 10, 16);
	innerWalls[4] = new Object(new CSolidCube);
	innerWalls[4]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 0.4f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[4]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[4]->SetDiffuseMap("texture/glass_diffuse.png");
	innerWalls[4]->SetNormalMap("texture/glass_normal.png");
	innerWalls[4]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[4]->Init(pos, rot, scale);
	innerWalls[4]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

	//verticle back
	pos = vec3(0, 5.01, 16.01);	rot = vec3(0);	scale = vec3(1, 10, 8);
	innerWalls[5] = new Object(new CSolidCube);
	innerWalls[5]->shape->setMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.85f, 0.85f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	innerWalls[5]->shape->setKaKdKsShini(0, 0.8f, 0.5f, 1);
	innerWalls[5]->SetDiffuseMap("texture/tile_diffuse.png");
	innerWalls[5]->SetNormalMap("texture/tile_normal.png");
	innerWalls[5]->SetTexShaderMode(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP, ShadingMode::PHONG_SHADING);
	innerWalls[5]->Init(pos, rot, scale);
	innerWalls[5]->InitCollision(new Collision(vec3(0), 1, 1, 1)); // collision not implement rotate, so use fake data

}


bool DetectCollision() 
{
	auto camera = CCamera::create();
	if (Cube->collision->IsCollide(camera->collision)) {
		return true;
	}
	for (auto innerWall : innerWalls) {
		if (innerWall->collision->IsCollide(camera->collision)) {
			return true;
		}
	}
	return false;
}

bool triggerThisTime = false;
void DetectCollider() 
{
	auto camera = CCamera::create();
	if (!triggerThisTime && Torii->collision->IsCollide(camera->collision) && Torii->collision->IsTirgger()) {
		g_bAutoRotating = !g_bAutoRotating;
		triggerThisTime = true;
	}
	else if(triggerThisTime && !Torii->collision->IsCollide(camera->collision)) {
		triggerThisTime = false;
	}
}


void UpdateLightColor(float delta) 
{
	if (g_fLightR > 0.05f && g_fLightB <= 0.05f) {
		g_fLightR -= 0.2f * delta;
		g_fLightG += 0.2f * delta;
	}
	else if (g_fLightG > 0.05f && g_fLightR <= 0.05f) {
		g_fLightG -= 0.2f * delta;
		g_fLightB += 0.2f * delta;
	}
	else if (g_fLightB > 0.05f && g_fLightG <= 0.05f) {
		g_fLightB -= 0.2f * delta;
		g_fLightR += 0.2f * delta;
	}
	g_Light1.diffuse = color4(g_fLightR, g_fLightG, g_fLightB, 1.0f);
	lightObj->shape->setColor(g_Light1.diffuse);
}