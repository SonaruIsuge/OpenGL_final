
#include "InputManager.h"


InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() 
{
	_mouseX = 0;
	_mouseY = 0;
	_bLeftClick = false;
	_bRightClick = false;

	isAnyKeyPressed = false;	
	keyPressed = '\0';
}


InputManager* InputManager::getInstance() 
{
	if(_instance == nullptr)
	{
		_instance = new InputManager;
	}

	return _instance;
}


void InputManager::onMouseMove(int x, int y) 
{
	_mouseX = x;
	_mouseY = y;
}


void InputManager::onMouseClick(int button, int state) 
{
	//printf("(%d, %d)\n", button, state);
	switch (button)
	{
	case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
		if (state == GLUT_DOWN)
			_bLeftClick = true;
		else if (state == GLUT_UP)
			_bLeftClick = false;
		break;

	case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 
		if (state == GLUT_DOWN);
		break;

	case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
		if (state == GLUT_DOWN)
			_bRightClick = true;
		else if (state == GLUT_UP)
			_bRightClick = false;

	default:
		break;
	}
}


void InputManager::onKeyPressed(unsigned char key) 
{
	keyPressed = key;
	isAnyKeyPressed = true;
}


int InputManager::getMouseX() { return _mouseX; }


int InputManager::getMouseY() { return _mouseY; }


bool InputManager::isMouseClick(int button) 
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		return _bLeftClick;
		break;

	case GLUT_RIGHT_BUTTON: 
		return _bRightClick;
		break;

	default:
		return false;
		break;
	}
}


bool InputManager::isKeyClick(unsigned char key)
{
	return isAnyKeyPressed && keyPressed == key;
}


void InputManager::ReleaseKey(unsigned char key)
{
	if(keyPressed == key) {
		keyPressed = '\0';
		isAnyKeyPressed = false;
	}
}


InputManager::~InputManager() {}


void InputManager::destroy() {
	if (_instance != nullptr) delete _instance;
}