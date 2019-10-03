
#include "glm/glm.hpp"	// glm 기본 함수
#include "glm/ext.hpp"	// glm 확장 함수
#include <math.h>	// c/c++ 기본 수학함수 (cos, sin 등)
#include <iostream>	// c++ 표준입출력 (cout)
#include <string>	// c++ 기본 string 클래스


int main(int argc, char** argv)
{
	glm::vec3 a(1.0f, 2.0f, 3.0f);
	glm::vec3 b(4.0f, 5.0f, 6.0f);
	float s = 0.5f;

	std::cout << "a = " << glm::to_string(a) << std::endl;
	std::cout << "b = " << glm::to_string(b) << std::endl;
	std::cout << "s = " << s << std::endl;
	std::cout << std::endl;


	glm::vec3 a1 = s * a + b;
	float a2 = glm::length(a);
	float a3 = glm::length(a-b);
	glm::vec3 a4 = glm::normalize(a - b);
	float a5 = glm::length(glm::normalize(a + b));
	float a6 = glm::dot(a, b);
	float a7 = a6 / (glm::length(a) * glm::length(b));
	glm::vec3 a8 = glm::cross(a, b);
	glm::vec3 a9 = glm::cross(b, a);
	glm::vec3 a10 = glm::normalize(a8);
	float a11 = glm::length(a8) / (glm::length(a) * glm::length(b)); 
	
	std::cout << "sa + b = "		<< glm::to_string(a1) << std::endl;
	std::cout << "||a|| = "			<< a2 << std::endl;
	std::cout << "||a-b|| = "		<< a3 << std::endl;
	std::cout << "(a-b)^ = "		<< glm::to_string(a4) << std::endl;
	std::cout << "||(a+b)^|| = "	<< a5 << std::endl;
	std::cout << "dot(a, b) = "		<< a6 << std::endl;
	std::cout << "cos(theta) = "	<< a7 << std::endl;
	std::cout << "cross(a, b) = "	<< glm::to_string(a8) << std::endl;
	std::cout << "cross(b, a) = "	<< glm::to_string(a9) << std::endl;
	std::cout << "n^ = "			<< glm::to_string(a10) << std::endl;
	std::cout << "sin(theta) = "	<< a11 << std::endl;

	std::cout << std::endl;


	glm::vec4 p[5] = {
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(2.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(2.0f, 2.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 3.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 2.0f, 0.0f, 1.0f)
	};


	for ( int i=0; i<5; i++ )
	{
		std::cout << "p" << i << " = " << glm::to_string(p[i]) << std::endl;
	}
	std::cout << std::endl;

	glm::mat4 R = glm::rotate( glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f) );
	glm::mat4 S = glm::scale( glm::vec3(1.0f, 2.0f, 1.0f) );
	glm::mat4 T = glm::translate( glm::vec3(-5.0f, 0.0f, 0.0f) ); 

	for ( int i=0; i<5; i++ )
	{
		p[i] = T * S * R * p[i];
	}


	for ( int i=0; i<5; i++ )
	{
		std::cout << "p" << i << " = " << glm::to_string((glm::vec3)p[i]) << std::endl;
	}
	std::cout << std::endl;

	return 0;
}




