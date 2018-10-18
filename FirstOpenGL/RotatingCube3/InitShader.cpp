#include "InitShader.h"
#include <GL/glew.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

bool ReadSourceFromFile(const std::string &in_filename, std::string &out_src)
{
	out_src.clear();
	std::ifstream fin(in_filename.c_str());

	if ( fin.fail() )
	{
		std::cerr << "Unable to open file '" << in_filename << "'" << std::endl;
		return false;
	}
	else
	{
		while (true)
		{
			char c = char(fin.get());
			if (!fin)
				return fin.eof();
			out_src += c;
		}
	}
}



unsigned int  CreateFromSource(const std::string &v_shader_src, const std::string &f_shader_src)
{
	GLuint program = glCreateProgram();

	// vertex shader
	GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
	{
		const GLchar *v_src = v_shader_src.c_str();
		glShaderSource(v_shader, 1, &v_src, NULL);
		glCompileShader(v_shader);

		GLint compiled;
		glGetShaderiv(v_shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) 
		{
			GLsizei len;
			glGetShaderiv( v_shader, GL_INFO_LOG_LENGTH, &len );

			GLchar* log = new GLchar[len+1];
			glGetShaderInfoLog( v_shader, len, &len, log );
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete [] log;
			return 0;
		}
		glAttachShader(program, v_shader);
	}

	// fragment shader
	GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		const GLchar *f_src = f_shader_src.c_str();
		glShaderSource(f_shader, 1, &f_src, NULL);
		glCompileShader(f_shader);

		GLint compiled;
		glGetShaderiv(f_shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) 
		{
			GLsizei len;
			glGetShaderiv( f_shader, GL_INFO_LOG_LENGTH, &len );

			GLchar* log = new GLchar[len+1];
			glGetShaderInfoLog( f_shader, len, &len, log );
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete [] log;
			return 0;
		}
		glAttachShader(program, f_shader);
	}


	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) 
	{
		GLsizei len;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( program, len, &len, log );
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete [] log;

		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
		glDeleteProgram(program);

		return 0;
	}

	return program;
}


unsigned int  CreateFromFiles(const std::string &v_shader_file, const std::string &f_shader_file)
{
	std::string v_shader_src;
	std::string f_shader_src;

	
	if ( !ReadSourceFromFile(v_shader_file, v_shader_src) )
	{
		return 0;
	}

	if ( !ReadSourceFromFile(f_shader_file, f_shader_src) )
	{
		return 0;
	}

	int program_handle_ = CreateFromSource(v_shader_src, f_shader_src);

	return program_handle_;
}