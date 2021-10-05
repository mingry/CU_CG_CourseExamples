#include <iostream>
#include "GL/freeglut.h"


GLuint g_window_w = 500;
GLuint g_window_h = 500;


void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void SpeicalKeyboard(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Timer(int value);



int main(int argc, char** argv)
{

	///////////////////////////////////////////////////////////////////////////////
	//// freeglut 초기화, 원도우 생성하기. 
	//// Ref: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/GLUT
	///////////////////////////////////////////////////////////////////////////////

	// 1. 최기화
	glutInit(&argc, argv);

	// 2. 윈도우 모드 설정. 
	// 더블버퍼링, RGB 프레임버퍼, Depth 버퍼를 사용한다.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// 3. 윈도우 사이즈 설정 (가로, 세로 픽셀 수).
	glutInitWindowSize(g_window_w, g_window_h);

	// 4. OpenGL Context와 윈도우 생성.
	glutCreateWindow("Computer Graphics");

	// 5. Callback functions 
	// Ref: http://freeglut.sourceforge.net/docs/api.php#GlobalCallback
	//      http://freeglut.sourceforge.net/docs/api.php#WindowCallback
	//      https://www.opengl.org/resources/libraries/glut/spec3/node45.html
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpeicalKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutTimerFunc(1000, Timer, 0);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// 6. freeglut 윈도우 이벤트 처리 루프 시작. 윈도우가 닫힐때까지 무한루프.
	glutMainLoop();
	std::cout << "A";
	return 0;
}



/**
Display: 윈도우 화면이 업데이트 될 필요가 있을 때 호출되는 callback 함수.
윈도우 상에 최종 결과를 렌더링 하는 코드는 이 함수 내에 구현해야한다.
원도우가 처음 열릴 때, 윈도우 크기가 바뀔 때, 다른 윈도우에 의해 화면의 일부
또는 전체가 가려졌다가 다시 나타날 때 등 시스템이 해당 윈도우 내의 그림에 대한
업데이트가 필요하다고 판단하는 경우 자동으로 호출된다.
강제 호출이 필요한 경우에는 glutPostRedisplay() 함수를 호출하면된다.
이 함수는 불시에 빈번하게 호출된다는 것을 명심하고, 윈도우 상태 변화와 무관한
1회성 코드는 가능한한 이 함수 밖에 구현해야한다. 특히 메모리 할당, VAO, VBO 생성
등의 하드웨어 점유를 시도하는 코드는 특별한 이유가 없다면 절대 이 함수에 포함시키면
안된다. 예를 들어, 메시 모델을 정의하고 VAO, VBO를 설정하는 부분은 최초 1회만
실행하면되므로 main() 함수 등 외부에 구현해야한다. 정의된 메시 모델을 프레임 버퍼에
그리도록 지시하는 코드만 이 함수에 구현하면 된다.
만일, 이 함수 내에서 동적 메모리 할당을 해야하는 경우가 있다면 해당 메모리는 반드시
이 함수가 끝나기 전에 해제 해야한다.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node46.html#SECTION00081000000000000000
*/
void Display()
{
	std::cout << "Display() is called." << std::endl;
}



/**
Timer: 지정된 시간 후에 자동으로 호출되는 callback 함수.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#SECTION000819000000000000000
*/
void Timer(int value)
{
	std::cout << "Timer() is called." << std::endl;


	// 1000밀리세컨드(1초) 후에 Timer 함수가 다시 호출되로록 한다.
	// Timer 함수 가 동일한 시간 간격으로 반복 호출되게하여,
	// 애니메이션 효과를 표현할 수 있다.
	glutTimerFunc(1000, Timer, 0);
}



/**
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수.
 @param w, h는 각각 조정된 윈도우의 가로 크기와 세로 크기 (픽셀 단위).
ref: https://www.opengl.org/resources/libraries/glut/spec3/node48.html#SECTION00083000000000000000
*/
void Reshape(int w, int h)
{
	std::cout << "Reshape() is called. (width=" << w << ", height=" << h << ")" << std::endl;


}






/**
Mouse: 마우스 버튼이 입력될 때마다 자동으로 호출되는 함수.
 파라메터의 의미는 다음과 같다.
 @param button: 사용된 버튼의 종류
  GLUT_LEFT_BUTTON - 왼쪽 버튼
  GLUT_RIGHT_BUTTON - 오른쪽 버튼
  GLUT_MIDDLE_BUTTON - 가운데 버튼 (휠 버튼이 눌러졌을 때)
  '3' - 마우스 휠 (휠이 위로 돌아 갔음).
  '4' - 마우스 휠 (휠이 아래로 돌아 갔음).
 @param state: 버튼 액셩 종류
  GLUT_DOWN - 눌러 졌음
  GLUT_UP - 놓여 졌음
 @param x,y: 해당 이벤트 발생 시, 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node50.html#SECTION00085000000000000000
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
 @param x는 마우스 포인터의 x 좌표값
 @param y는 마우스 포인터의 y 좌표값.
 ref: https://www.opengl.org/resources/libraries/glut/spec3/node51.html#SECTION00086000000000000000
*/
void MouseMotion(int x, int y)
{
	std::cout << "mouse dragged : (" << x << ", " << y << ")" << std::endl;

}



/**
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
 @param key는 눌려진 키보드의 문자값.
 @param x,y는 현재 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000
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
	else if (key == 'q')
	{
		glutLeaveMainLoop();
	}
}




/**
SpeicalKeyboard: 문자값이 아닌 특수키가 입력 될 때마다 자동으로 호출되는 함수.
 @param key는 눌려진 키보드를 나타내는 매크로 값 (freeglut_std.h 참고).
 @param x,y는 현재 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node54.html#SECTION00089000000000000000
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