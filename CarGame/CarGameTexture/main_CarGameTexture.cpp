#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "CarGameTextureApp.h"
#include "IL/ilu.h"


GLuint g_window_w = 800;
GLuint g_window_h = 600;




int main(int argc, char** argv)
{

	ilInit();
	iluInit();

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutCreateWindow("Computer Graphics");
	
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutTimerFunc((unsigned int)(1000 /60), Timer, (1000 / 60));


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

	InitOpenGL();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();


	ClearOpenGLResource();

	return 0;
}








