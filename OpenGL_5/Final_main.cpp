// 
// Example 4 : Normal Map 的運作原理展示 
//  Normal Map 使用 vsNormalMapLighting fsNormalMapLighting, 此組 shader 只支援 diffuse+Normal Map
//  設定使用 Normal Map，透過 setTextureLayer(TextureMode::DIFFUSE_MAP | TextureMode::NORMAL_MAP);
//  
#include "Final/Application.h"


Application* application;

//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();

void init( void )
{
	application = new Application;
	application->Start();
}


//----------------------------------------------------------------------------
void GL_Display( void )
{
	application->Draw();

	glutSwapBuffers();	// 交換 Frame Buffer
}


void onFrameMove(float delta)
{
	application->Update(delta);

	GL_Display();
}


void Win_Keyboard( unsigned char key, int x, int y )
{
	application->OnKeyboardHit(key, x, y);
}


void Win_Keyboard_Up(unsigned char key, int x, int y)
{
	application->OnKeyboardRelease(key, x, y);
}


void Win_Mouse(int button, int state, int x, int y) 
{
	application->OnMouseClick(button, state, x, y);
}


void Win_SpecialKeyboard(int key, int x, int y) 
{
	application->OnKeyboardHit(key, x, y);
}


// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) 
{
	application->OnMouseMoveWithoutClick(x, y);
}


// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) 
{
	application->OnMouseMoveWithClick(x, y);
}


void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}


int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutKeyboardUpFunc(Win_Keyboard_Up);
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// 這一行會讓視窗被強迫關閉時，程式的執行會回到 glutMainLoop(); 的下一行
	glutMainLoop();
	application->Destroy();
    return 0;
}