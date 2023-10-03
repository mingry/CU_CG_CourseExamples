#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "RotatingCube2App.h"


GLuint g_window_w = 500;
GLuint g_window_h = 500;




int main(int argc, char** argv)
{

	//////////////////////////////////////////////////////////////////////////////////////
	//// 1. freeglut 초기화, 원도우 생성하기. 
	////    Ref: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/GLUT
	//////////////////////////////////////////////////////////////////////////////////////

	//// 1.1. 최기화
	glutInit(&argc, argv);
	
	//// 1.2. 윈도우 모드 설정. 
	// 더블버퍼링, RGB 프레임버퍼, Depth 버퍼를 사용한다.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//// 1.3. 윈도우 사이즈 설정 (가로, 세로 픽셀 수).
	glutInitWindowSize(g_window_w, g_window_h);

	//// 1.4. OpenGL Context와 연결된 윈도우 생성.
	glutCreateWindow("Computer Graphics");
	
	//// 1.5. Callback functions 
	// Ref: http://freeglut.sourceforge.net/docs/api.php#GlobalCallback
	//      http://freeglut.sourceforge.net/docs/api.php#WindowCallback
	//      https://www.opengl.org/resources/libraries/glut/spec3/node45.html
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc((unsigned int)(1000 /60), Timer, 0);



	//////////////////////////////////////////////////////////////////////////////////////
	//// 2. GLEW 초기화. 
	////    Ref: http://glew.sourceforge.net/basic.html
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}



	//////////////////////////////////////////////////////////////////////////////////////
	//// 3. Shader Programs 등록
	//// 4. OpenGL 설정
	//// 5. VAO, VBO 생성
	//////////////////////////////////////////////////////////////////////////////////////
	InitOpenGL();


	//// 1.6. freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();


	ClearOpenGLResource();

	return 0;
}








