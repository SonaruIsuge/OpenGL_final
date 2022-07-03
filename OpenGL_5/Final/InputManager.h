#pragma once
#include "../header/Angel.h"


#define ESCAPE_KEY 033
#define SPACE_KEY 32
#define KEYCODE_A 65
#define KEYCODE_a 97
#define KEYCODE_S 83
#define KEYCODE_s 115
#define KEYCODE_D 68
#define KEYCODE_d 100
#define KEYCODE_W 87
#define KEYCODE_w 119
#define KEYCORD_L 76
#define KEYCORD_l 108
#define KEYCODE_R 82
#define KEYCODE_r 114
#define KEYCODE_G 71
#define KEYCODE_g 103
#define KEYCODE_B 66
#define KEYCODE_b 98


class InputManager 
{
private:
	static InputManager* _instance;

	int _mouseX;
	int _mouseY;
	bool _bLeftClick;
	bool _bRightClick;
	//bool _bExitGame;
	//bool _bSpaceClick;
	//bool isAnyMousePressed;
	//int mousePressed;
	bool isAnyKeyPressed;
	unsigned char keyPressed;


public:
	static InputManager* getInstance();

	InputManager();
	~InputManager();
	void onMouseMove(int x, int y);
	void onMouseClick(int button, int state);
	void onKeyPressed(unsigned char key);

	int getMouseX();
	int getMouseY();
	bool isMouseClick(int button);
	bool isKeyClick(unsigned char key);

	void ReleaseKey(unsigned char key);

	void destroy();
};