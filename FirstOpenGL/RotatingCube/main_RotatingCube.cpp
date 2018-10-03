#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


GLuint g_window_w = 500;
GLuint g_window_h = 500;


//////////////////////////////////////////////////////////////////////
//// Define Mesh Model
//////////////////////////////////////////////////////////////////////
const int g_num_vertices = 24;
const int g_num_triangles = 12;

GLfloat m_positions[g_num_vertices * 3] =
{  0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f,  0.25f,
  -0.25f, -0.25f,  0.25f,
   0.25f, -0.25f,  0.25f,
 						
   0.25f,  0.25f,  0.25f,
   0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,
  -0.25f,  0.25f,  0.25f,
 						
   0.25f,  0.25f,  0.25f,
   0.25f, -0.25f,  0.25f,
   0.25f, -0.25f, -0.25f,
   0.25f,  0.25f, -0.25f,
 						
   0.25f,  0.25f, -0.25f,
   0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f,  0.25f, -0.25f,
 						
   0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f,  0.25f,
  -0.25f, -0.25f, -0.25f,
   0.25f, -0.25f, -0.25f,
 						
  -0.25f,  0.25f,  0.25f,
  -0.25f,  0.25f, -0.25f,
  -0.25f, -0.25f, -0.25f,
  -0.25f, -0.25f,  0.25f
};

GLfloat m_normals[g_num_vertices * 3] =
{  0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   0.f,  0.f,  1.f,
   		 	   	   
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   0.f,  1.f,  0.f,
   		 	   	   
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   1.f,  0.f,  0.f,
   		 	   	   
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,
   0.f,  0.f, -1.f,

   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,
   0.f, -1.f,  0.f,

  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f,
  -1.f,  0.f,  0.f
};

GLfloat m_colors[g_num_vertices * 4] =
{ 1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,
					
  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f,

  1.f, 0.f, 0.f, 1.f,
  0.f, 1.f, 0.f, 1.f,
  0.f, 0.f, 1.f, 1.f,
  1.f, 1.f, 0.f, 1.f
};

GLuint m_indices[g_num_triangles * 3] =
{
   0, 1, 2,
   0, 2, 3,
   
   4, 5, 6,
   4, 6, 7,
   
   8,  9, 10,
   8, 10, 11,
  
  12, 13, 14,
  12, 14, 15,
  
  16, 17, 18,
  16, 18, 19,
  
  20, 21, 22,
  20, 22, 23
};


//////////////////////////////////////////////////////////////////////
//// Define Shader Programs
//////////////////////////////////////////////////////////////////////

GLuint f_shader_id;
GLuint v_shader_id;
GLuint s_program_id;

// vertex shader source code
const GLchar * v_shader_source =
R"(
	#version 330

	layout (location=0) in vec3 vs_position;
	layout (location=1) in vec3 vs_normal;
	layout (location=2) in vec4 vs_color;

	uniform mat4 proj_matrix;
	uniform mat4 view_matrix;
	uniform mat4 model_matrix;

	out vec3 fs_normal;
	out vec4 fs_color;

	void main()
	{
		gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vs_position, 1.f);
		fs_normal = vec3(view_matrix * model_matrix * vec4(vs_normal, 0.f));
		fs_color = vs_color;
	}
		
)";

// fragament shader source code
const GLchar * f_shader_code =
R"(
	#version 330

	in vec4 fs_color;
	in vec3 fs_normal;

	uniform int shading_mode = 1;

	layout (location = 0) out vec4 color;
	
	void main()
	{
		vec3 N = normalize(fs_normal);

		vec3 L = vec3(1.f, 1.f, 1.f);
		L = normalize(L);

		float diffuse = clamp(dot(L, N), 0.2f, 1.f);

		if ( shading_mode == 1 )
			color = vec4(diffuse, diffuse, diffuse, 1.f) * fs_color;
		else if ( shading_mode == 2 )
			color = vec4(diffuse, diffuse, diffuse, 1.f);
	}
		
)";

//////////////////////////////////////////////////////////////////////
//// Declare VAO, VBO variables
//////////////////////////////////////////////////////////////////////
GLuint g_vao_id;
GLuint g_vbo_position_id;
GLuint g_vbo_normal_id;
GLuint g_vbo_color_id;
GLuint g_index_buffer_id;

// Index Buffer Object
GLuint index_buffer_id;


//////////////////////////////////////////////////////////////////////
//// Declare callback functions
//////////////////////////////////////////////////////////////////////
void Reshape(int w, int h);
void Display();
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);



//////////////////////////////////////////////////////////////////////
//// Animation Parameters
//////////////////////////////////////////////////////////////////////
float g_y_angle = 0.f;


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
	////    Ref: https://www.khronos.org/opengl/wiki/Shader_Compilation
	//////////////////////////////////////////////////////////////////////////////////////

	//// Vertex shader program
	//// 3.1. Vertex shader 객체 생성.
	GLuint v_shader_id = glCreateShader(GL_VERTEX_SHADER);

	//// 3.2. Vertex shader 소스코드 입력.
	glShaderSource(v_shader_id, 1, &v_shader_source, NULL);

	//// 3.3. Vertex shader 소스코드 컴파일.
	glCompileShader(v_shader_id);

	//// 3.4. 컴파일 에러 발생시 에러 메세지 출력.
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


	//// Fragment shader program 
	//// 3.5. Fragment shader 객체 생성.
	GLuint f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	//// 3.6. Fragment shader 소스코드 입력.
	glShaderSource(f_shader_id, 1, &f_shader_code, NULL);

	//// 3.7. Fragment shader 소스코드 컴파일.
	glCompileShader(f_shader_id);

	//// 3.8. 컴파일 에러 발생시 에러 메세지 출력.
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

	//// Shader Program (vertex shader + fragment shader)
	//// 3.9. Shader Program 객채 생성.
	s_program_id = glCreateProgram();

	//// 3.10. Vertex shader와 Fragment shader를 Shader Program 객체에 등록.
	glAttachShader(s_program_id, v_shader_id);
	glAttachShader(s_program_id, f_shader_id);

	//// 3.11. Shader Program 링크
	glLinkProgram(s_program_id);

	//// 3.12. Shader Program 사용시작
	glUseProgram(s_program_id);






	////////////////////////////////////////////////////////////////////////////////////
	//// 4. OpenGL 설정
	//////////////////////////////////////////////////////////////////////////////////////

	glViewport(0, 0, (GLsizei)g_window_w, (GLsizei)g_window_h);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	

	

	
	////////////////////////////////////////////////////////////////////////////////////
	//// 5. VAO, VBO 생성
	////    Ref: https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
	///////////////////////////////////////////////////////////////////////////////////

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &g_vao_id);
	glBindVertexArray(g_vao_id);

	
	//// 5.2. vertex positions 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_position_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_position_id);

	//// 5.3. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_positions, GL_STATIC_DRAW);

	//// 5.4. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	//// 5.5. vertex normal vectors 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal_id);

	//// 5.6. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_normals, GL_STATIC_DRAW);

	//// 5.7. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	//// 5.8. vertex colors 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_color_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_color_id);

	//// 5.9. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_colors, GL_STATIC_DRAW);

	//// 5.10. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	//// 5.11. Index Buffer 객체 생성 및 데이터 입력
	glGenBuffers(1, &g_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * g_num_triangles, m_indices, GL_STATIC_DRAW);

	


	//// 1.6. freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

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
	// 전체 화면을 지운다.
	// glClear는 Display 함수 가장 윗 부분에서 한 번만 호출되어야한다.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Vertex shader 의 matrix 변수들의 location을 받아온다.
	int m_proj_loc = glGetUniformLocation(s_program_id, "proj_matrix");
	int m_view_loc = glGetUniformLocation(s_program_id, "view_matrix");
	int m_model_loc = glGetUniformLocation(s_program_id, "model_matrix");


	// Projection Transform Matrix 설정.
	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f), 1.f, 0.01f, 1000.f);
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Camera Transform Matrix 설정.
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(2.f, 1.5f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));


	// 첫번째 큐브의 Transform Matrix 설정.
	glm::mat4 T1 = glm::rotate(glm::radians(g_y_angle), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T1));

	glBindVertexArray(g_vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);


	//// 두번째 큐브의 Transform Matrix 설정.
	//glm::mat4 trans_x = glm::translate(glm::vec3(0.5f, 0.f, 0.f));
	//glm::mat4 scale_half = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
	//glm::mat4 T2 = T1 * trans_x * scale_half;
	//glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, glm::value_ptr(T2));

	//glBindVertexArray(g_vao_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	//glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);

	

	
	// flipping the double buffers
	// glutSwapBuffers는 항상 Display 함수 가장 아래 부분에서 한 번만 호출되어야한다.
	glutSwapBuffers();
}



/**
Timer: 지정된 시간 후에 자동으로 호출되는 callback 함수.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node64.html#SECTION000819000000000000000
*/
void Timer(int value)
{
	g_y_angle += 1.f;

	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수가 호출 된다.
	glutPostRedisplay();

	// 1/60 초 후에 Timer 함수가 다시 호출되로록 한다.
	// Timer 함수 가 동일한 시간 간격으로 반복 호출되게하여,
	// 애니메이션 효과를 표현할 수 있다
	glutTimerFunc((unsigned int)(1000 / 60), Timer, 0);
}



/**
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수.

@param w, h는 각각 조정된 윈도우의 가로 크기와 세로 크기 (픽셀 단위).
ref: https://www.opengl.org/resources/libraries/glut/spec3/node48.html#SECTION00083000000000000000
*/
void Reshape(int w, int h)
{
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;

	glutPostRedisplay();
}

/**
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수.
@param key는 눌려진 키보드의 문자값.
@param x,y는 현재 마우스 포인터의 좌표값.
ref: https://www.opengl.org/resources/libraries/glut/spec3/node49.html#SECTION00084000000000000000

*/
void Keyboard(unsigned char key, int x, int y)
{
	// keyboard '1' 이 눌려졌을 때.
	if (key == '1')
	{
		// Fragment shader에 정의 되어있는 'shading_mode' 변수의 location을 받아온다.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' 값으로 1을 설정.
		glUniform1i(shading_mode_loc, 1);


		// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
		// 결과적으로 Display() 함수를 호출하게 된다.
		glutPostRedisplay();
	}

	// keyboard '2' 가 눌려졌을 때.
	else if (key == '2')
	{
		// Fragment shader에 정의 되어있는 'shading_mode' 변수의 location을 받아온다.
		int shading_mode_loc = glGetUniformLocation(s_program_id, "shading_mode");

		// 'shading_mode' 값으로 2를 설정.
		glUniform1i(shading_mode_loc, 2);


		// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
		// 결과적으로 Display() 함수를 호출하게 된다.
		glutPostRedisplay();
	}
}








