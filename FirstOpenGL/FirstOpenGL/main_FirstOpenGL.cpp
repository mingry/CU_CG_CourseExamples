#include <iostream>
#include "GL/glew.h"
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
	glutTimerFunc((unsigned int)(1000 /60), Timer, 0);





	///////////////////////////////////////////////////////////////////////
	//// Initialize GLEW
	glewExperimental = TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	

	///////////////////////////////////////////////////////////////////////
	//// Vertex shader program 

	// create vertex shader and obejct the id.
	GLuint v_shader_id = glCreateShader(GL_VERTEX_SHADER);

	// vertex shader source code
	const GLchar * v_shader_source = 
		R"(
			#version 330

			layout (location=0) in vec4 vs_position;
			layout (location=1) in vec4 vs_normal;
			layout (location=2) in vec4 vs_color;

			//uniform mat4 projection_matrix;
			//uniform mat4 modelview_matrix;

			out vec4 fs_color;

			void main()
			{
				gl_Position = vs_position; //projection_matrix * modelview_matrix * vs_position;
				fs_color = vs_color;
			}
		
		)";

	glShaderSource(v_shader_id, 1, &v_shader_source, NULL);
	glCompileShader(v_shader_id);

	{
		GLint compiled;
		glGetShaderiv(v_shader_id, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(v_shader_id, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(v_shader_id, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return 0;
		}
	}


	///////////////////////////////////////////////////////////////////////
	//// Fragment shader program 

	// create vertex shader and obejct the id.
	GLuint f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// fragament shader source code
	const GLchar * f_shader_code =
		R"(
			#version 330

			in vec4 fs_color;
	
			layout (location = 0) out vec4 color;

			void main()
			{
				color = fs_color;
			}
		
		)";

	glShaderSource(f_shader_id, 1, &f_shader_code, NULL);
	glCompileShader(f_shader_id);

	{
		GLint compiled;
		glGetShaderiv(f_shader_id, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(f_shader_id, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(f_shader_id, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return 0;
		}
	}


	///////////////////////////////////////////////////////////////////////
	//// Shader Program (vertex shader + fragment shader)

	// Create shader program and obtain the id. 
	GLuint s_program_id = glCreateProgram();

	// Attache vertex and fragment shaders to the program
	glAttachShader(s_program_id, v_shader_id);
	glAttachShader(s_program_id, f_shader_id);

	glLinkProgram(s_program_id);
	glUseProgram(s_program_id);





	///////////////////////////////////////////////////////////////////////
	//// Initialize OpenGL

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);
	glOrthof(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f);
	
	// 배경색을 정한다.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	

	///////////////////////////////////////////////////////////////////////
	//// Mesh Model
	GLfloat m_positions[g_num_vertices * 4] = 
	{     0.f,  0.f,  0.f, 1.f
		, 0.5f, 0.f,  0.f, 1.f
		, 0.5f, 0.5f, 0.f, 1.f
		, 0.f,  0.5f, 0.f, 1.f
	};

	GLfloat m_normals[g_num_vertices * 4] =
	{     0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
		, 0.f, 0.f, 1.f, 0.f
	};

	GLfloat m_colors[g_num_vertices * 4] =
	{     1.f, 0.f, 0.f, 1.f
		, 0.f, 1.f, 0.f, 1.f
		, 0.f, 0.f, 1.f, 1.f
		, 1.f, 1.f, 0.f, 1.f
	};


	GLuint m_indices[g_num_triangles * 3] =
	{
		0, 1, 2,
		0, 2, 3
	};


	///////////////////////////////////////////////////////////////////////
	//// Vertex Array Object
	// VAO를 생성하고 자동으로 부여된 이름을 g_two_tri_vao에 담는다.
	glGenVertexArrays(1, &g_vao_id);
	glBindVertexArray(g_vao_id);


	//// VBO for positions
	// 꼭지점(vertex)의 좌표값을 저장하기 위한 VBO를 생성하고 자동으로 부여된 VBO의 이름을 g_two_tri_vbo_pos에 담는다.
	glGenBuffers(1, &g_vbo_position_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_position_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_positions, GL_STATIC_DRAW);	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &g_vbo_normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &g_vbo_color_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_color_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	// Index Buffer Object
	glGenBuffers(1, &g_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * g_num_triangles, m_indices, GL_STATIC_DRAW);





	glutMainLoop();

	return 0;
}



/**
Display: 그림을 그리기 위해 호출되는 callback 함수.
화면에 그림이 새로 그려질 필요가 있을 때마다 자동으로 반복해서 여러 번 호출된다.
(예. 다른 윈도우에 의해 화면의 일부분이 가려졌다가 다시 나타나는 경우.)
반복적으로 호출된다는 것을 명심하고, 불필요한 내용은 절대 이곳에 넣어서는 안된다.
예를 들어, VAO나 VBO를 생성하는 코드를 이곳에 넣어서는 안된다. 이 곳에는 이미 생성된
VAO와 VBO를 사용해서 그림을 그리는 코드만 포함되어야 한다.

이 예제의 경우에는 g_draw_mode가 변함에 따라 다른 그림이 나타나도록 하였다.
*/
void Display()
{
	// 전체 화면을 지운다.
	// glClear는 Display 함수 가장 윗 부분에서 한 번만 호출되어야한다.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindVertexArray(g_vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);

	
	// 더블버퍼링 수행을 위한 함수 호출.
	// glutSwapBuffers는 항상 Display 함수 가장 아래 부분에서 한 번만 호출되어야한다.
	glutSwapBuffers();
}



/**
Timer: 애니메이션을 위해 주기적으로 호출되는 함수.
이 프로그램에서는 1초에 60번 호출되도록 설계한다.
흘러간 시간을 업데이트하고 이를 바탕으로 선풍기 모터의 각도를 새로 계산한다.
*/
void Timer(int value)
{

	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수를 호출하게 된다.
	glutPostRedisplay();

	// 1/60 초 후에 Timer 함수가 다시 호출되로록 한다.
	glutTimerFunc((unsigned int)(1000 / 60), Timer, 0);
}



/**
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수.
w와 h는 각각 조정된 윈도우의 가로 크기와 세로 크기를 나타낸다 (픽셀 단위).
화면 크기가 변화하면, Viewport의 크기, 카메라의 가로세로 비율 등을 다시 설정해야한다.
*/
void Reshape(int w, int h)
{
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;


	//  w : window width   h : window height
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);

}









/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input



/**
Mouse: 마우스 버튼이 입력될 때마다 자동으로 호출되는 함수.
파라메터의 의미는 다음과 같다.
1. button: 사용된 버튼의 종류
GLUT_LEFT_BUTTON - 왼쪽 버튼
GLUT_RIGHT_BUTTON - 오른쪽 버튼
GLUT_MIDDLE_BUTTON - 가운데 버튼 (휠이 눌러졌을 때)
3 - 마우스 휠 (휠이 위로 돌아 갔음).
4 - 마우스 휠 (휠이 아래로 돌아 갔음).
2. state: 조작 상태
GLUT_DOWN - 눌러 졌음
GLUT_UP - 놓여졌음
3. x,y: 조작이 일어났을 때, 마우스 포인터의 좌표값.
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
MouseMotion: 마우스 포인터가 움직일 때마다 자동으로 호출되는 함수.
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.
*/
void MouseMotion(int x, int y)
{
	std::cout << "mouse dragged : (" << x << ", " << y << ")" << std::endl;

}



/**
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
파라메터 key 는 눌려진 키보드의 문자값을 나타낸다.
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.

이 예제에서는 키보드 '1', '2', '3', '4' 가 눌려지면 g_draw_mode를 변경한다.
*/
void Keyboard(unsigned char key, int x, int y)
{
	std::cout << "key '" << key << "' pushed : (" << x << ", " << y << ")" << std::endl;
	


	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수를 호출하게 된다.
	glutPostRedisplay();
}




/**
SpeicalKeyboard: 문자값으로 표현하기 어려운 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
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

	glutPostRedisplay();
}






