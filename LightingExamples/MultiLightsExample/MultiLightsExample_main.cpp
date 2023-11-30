
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "MultiLightsExampleApp.h"


GLuint g_window_w = 800;
GLuint g_window_h = 600;


int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(g_window_w, g_window_h);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutCreateWindow("Computer Graphics");


	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

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

	glutMainLoop();

	ClearOpenGLResource();

	return 0;
}

