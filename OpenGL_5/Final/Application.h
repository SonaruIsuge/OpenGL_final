#pragma once

#include "../Header/Angel.h"



#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 32


class Application
{
private:


public:
	Application();
	~Application();

	void Start();
	void Draw();
	void Update(float delta);
	void OnKeyboardHit(unsigned char key, int x, int y);
	void OnKeyboardRelease(unsigned char key, int x, int y);
	void OnMouseClick(int button, int state, int x, int y);
	void OnMouseMoveWithoutClick(int x, int y);
	void OnMouseMoveWithClick(int x, int y);
	void Destroy();
};