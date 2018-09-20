#include <iostream>
#include "GL/freeglut.h"


GLuint g_window_w = 500;
GLuint g_window_h = 500;


const int g_num_vertices = 4;
const int g_num_triangles = 2;


GLuint g_vao_id;
GLuint g_vbo_position_id;
GLuint g_vbo_normal_id;
GLuint g_vbo_color_id;
GLuint g_index_buffer_id;


// Index Buffer Object
GLuint index_buffer_id;

void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void SpeicalKeyboard(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Timer(int value);



int main(int argc, char** argv)
{


	///////////////////////////////////////////////////////////////////////
	//// Create and Initialize Window
	glutInit(&argc, argv);
	
	// specifies display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// specifies the size, in pixels, of your window
	glutInitWindowSize(g_window_w, g_window_h);

	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	// creates a window with an OpenGL context
	glutCreateWindow("Computer Graphics");
	
	// Callback functions
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpeicalKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutTimerFunc(1000, Timer, 0);



	glutMainLoop();

	return 0;
}



/**
Display: 그림을 그리기 위해 호출되는 callback 함수.
화면에 그림이 새로 그려질 필요가 있을 때마다 자동으로 반복해서 여러 번 호출된다.
(예. 다른 윈도우에 의해 화면의 일부분이 가려졌다가 다시 나타나는 경우.)
반복적으로 호출된다는 것을 명심하고, 불필요한 내용은 절대 이곳에 넣어서는 안된다.
*/
void Display()
{
	std::cout << "Display() is called." << std::endl;
}



/**
Timer: glutTimerFunc가 호출 된 후 일정 시간 후에 호출되는 callback 함수.
*/
void Timer(int value)
{
	std::cout << "Timer() is called." << std::endl;
	

	// 1000밀리세컨드(1초) 후에 Timer 함수가 다시 호출되로록 한다.
	glutTimerFunc(1000, Timer, 0);
}



/**
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수.
w와 h는 각각 조정된 윈도우의 가로 크기와 세로 크기를 나타낸다 (픽셀 단위).
*/
void Reshape(int w, int h)
{
	std::cout << "Reshape() is called. (width=" << w << ", height=" << h << ")" << std::endl;


}









/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input



/**
Mouse: 마우스 버튼이 입력될 때마다 자동으로 호출되는 함수.
파라메터의 의미는 다음과 같다.
1. button: 사용된 버튼의 종류
  GLUT_LEFT_BUTTON - 왼쪽 버튼
  GLUT_RIGHT_BUTTON - 오른쪽 버튼
  GLUT_MIDDLE_BUTTON - 가운데 버튼 (휠 버튼이 눌러졌을 때)
  3 - 마우스 휠 (휠이 위로 돌아 갔음).
  4 - 마우스 휠 (휠이 아래로 돌아 갔음).
2. state: 버튼 액셩 종류
  GLUT_DOWN - 눌러 졌음
  GLUT_UP - 놓여 졌음
3. x,y: 해당 이벤트 발생 시, 마우스 포인터의 좌표값.
*/
void Mouse(int button, int state, int x, int y)
{


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "mouse left button pushed : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		std::cout << "mouse left button released : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "mouse right button pushed : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		std::cout << "mouse right button released : (" << x << ", " << y << ")" << std::endl;
	}
	else if (button == 3)
	{
		std::cout << "mouse wheel up" << std::endl;
	}
	else if (button == 4)
	{
		std::cout << "mouse wheel down" << std::endl;
	}

}





/**
MouseMotion: 마우스 버튼이 눌려진 상태에서 포인터가 움직일 때 (드래그할 때)마다 자동으로 호출되는 함수.
 파라메터 x,y는 마우스 포인터의 좌표값.
*/
void MouseMotion(int x, int y)
{
	std::cout << "mouse dragged : (" << x << ", " << y << ")" << std::endl;

}



/**
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
파라메터 key 는 눌려진 키보드의 문자값을 나타낸다.
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.

이 예제에서는 키보드 'r' 가 눌려지면 그림을 업데이트 할 것을 요청한다.
*/
void Keyboard(unsigned char key, int x, int y)
{
	std::cout << "key '" << key << "' pushed : (" << x << ", " << y << ")" << std::endl;
	
	if (key == 'r')
	{
		// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
		// 결과적으로 Display() 함수를 호출하게 된다.
		glutPostRedisplay();
	}
}




/**
SpeicalKeyboard: 문자값이 아닌 특수키가 입력 될 때마다 자동으로 호출되는 함수.
파라메터 key 는 눌려진 키보드를 나타내는 매크로 값 (freeglut_std.h 참고).
파라메터 x,y는 현재 마우스 포인터의 좌표값.
*/
void SpeicalKeyboard(int key, int x, int y)
{
	switch (key)
	{
		std::cout << key << std::endl;
		/* fill here */
	case GLUT_KEY_DOWN:
		std::cout << "down" << std::endl;
		break;

	case GLUT_KEY_UP:
		std::cout << "up" << std::endl;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left" << std::endl;
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right" << std::endl;
		break;

	}
}






