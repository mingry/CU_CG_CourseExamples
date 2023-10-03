
#include "glm/glm.hpp"	// glm 기본 함수
#include "glm/ext.hpp"	// glm 확장 함수
#include <math.h>	// c/c++ 기본 수학함수 (cos, sin 등)
#include <iostream>	// c++ 표준입출력 (cout)
#include <string>	// c++ 기본 string 클래스


int main(int argc, char** argv)
{
	////////////////////////////////////////////////////////////////////////////
	//// c++ string 클래스 사용.
	// (read) http://tcpschool.com/cpp/cpp_string_stringClass
	std::string str1 = "aaa";
	std::string str2 = "bbb";
	std::string str3 = str1 + str2;


	
	////////////////////////////////////////////////////////////////////////////
	//// c++ 표준출력 cout의 사용. 
	// 기본 변수 (int, float 등) 또는 표준출력 방식이 정의되어있는 class 객체 (std::string 등)만을 출력할 수 있다.
	// 출력할 값이나 변수 사이를 << 기호로 구분한다.
	// std::endl 은 줄바꿈 문자이다. "\n"와 같다.
	// (read) http://tcpschool.com/cpp/cpp_intro_iostream
	std::cout << "Hello world" << std::endl;
	std::cout << "str1 + str2 = " << str3 << std::endl;	
	std::cout << std::endl;	// 줄바꿈 문자 출력.
	std::cout << std::endl;	// 줄바꿈 문자 출력.




	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////// Using GML
	// (read) https://glm.g-truc.net/0.9.9/api/modules.html

	////////////////////////////////////////////////////////////////////////////
	//// GLM vec3 클래스 사용
	// (read) https://glm.g-truc.net/0.9.9/api/a00699.html
	std::cout << "* GLM 3D Vector" << std::endl;

	// 변수 선언 및 초기화
	glm::vec3 v0(1, 2, -3);		// 생성자 호출을 통한 초기화

	glm::vec3 v1;		// 변수 선언
	v1.x = -4;			// x값 설정
	v1.y = 5;
	v1.z = 6;

	// 벡터 더하기
	glm::vec3 v2 = v0 + v1;

	// 실수(float) 곱하기 벡터
	glm::vec3 v3 = 2.5f * v1;

	// Normalizing 계산
	glm::vec3 unit_v1 = glm::normalize(v1);

	// Length 계산
	float length_v1 = glm::length(v1);

	// Dot Product
	float dot_v0_v1 = glm::dot(v0, v1);

	// Cross Product
	glm::vec3 cross_v0_v1 = glm::cross(v0, v1);


	// Print out
	std::cout << "v0 = " << glm::to_string(v0) << std::endl << std::endl;
	std::cout << "v1 = " << glm::to_string(v1) << std::endl << std::endl;
	std::cout << "v0 + v1 = " << glm::to_string(v2) << std::endl << std::endl;
	std::cout << "2.5 * v1 = " << glm::to_string(v3) << std::endl << std::endl;
	std::cout << "Length of v1 = " << length_v1 << std::endl << std::endl;
	std::cout << "Dot product of v0 and v1 = " << dot_v0_v1 << std::endl << std::endl;
	std::cout << "Cross product of v0 and v1 = " << glm::to_string(cross_v0_v1) << std::endl << std::endl;




	////////////////////////////////////////////////////////////////////////////
	//// GLM Matrix 클래스 사용
	// (read) https://glm.g-truc.net/0.9.9/api/a00701.html
	std::cout << std::endl << std::endl << "* GLM Matrix" << std::endl;

	// 3x3 단위 행렬 (identity matrix)
	glm::mat3 m0(1.0f);

	// 3x3 임의의 값 행렬 matrix
	glm::mat3 m1(1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 0.0f, 9.0f);


	// Assign 후 (1, 2) 요소값 변경
	glm::mat3 m2 = m1;
	m2[1][2] = -5.0f;


	// 역행렬 (Matrix Inverse)
	glm::mat3 m1_inverse = glm::inverse(m1);

	// 전치행렬 (Matrix Transpose)
	glm::mat3 m1_transpose = glm::transpose(m1);

	// 행렬 곱하기 (Matrix Multiplicaqtion)
	glm::mat3 m1xm2 = m1 * m2;


	// 행렬 곱하기 벡터 (Matrix * Vector)
	glm::vec3 m1xv1 = m1 * v1;


	// Print out
	std::cout << "m0 = " << glm::to_string(m0) << std::endl << std::endl;
	std::cout << "m1 = " << glm::to_string(m1) << std::endl << std::endl;
	std::cout << "m2 = " << glm::to_string(m2) << std::endl << std::endl;
	std::cout << "Inverse of m1 = " << glm::to_string(m1_inverse) << std::endl << std::endl;
	std::cout << "Transpose of m1 = " << glm::to_string(m1_transpose) << std::endl << std::endl;
	std::cout << "m1 * m2 = " << glm::to_string(m1xm2) << std::endl << std::endl;
	std::cout << "m1 * v1 = " << glm::to_string(m1xv1) << std::endl << std::endl;





	////////////////////////////////////////////////////////////////////////////
	//// GLM 을 이용한 기하학 변환 (Transform) 계산 
	// Transformation in Homogeneous Coordinates
	// (read) https://glm.g-truc.net/0.9.9/api/a00704.html
	std::cout << std::endl << std::endl << "* GLM Transform" << std::endl;

	glm::vec4 p0(1.f, 2.f, 3.f, 1.f);	// point
	glm::vec4 p1(-3.f, -2.f, -3.f, 1.f);	// point
	glm::vec4 u0(2.f, 1.f, 3.f, 0.f);	// vector
	glm::vec4 u0_n = glm::normalize(u0);	// unit vector

	// transform matrix
	glm::mat4 R = glm::rotate(glm::radians(30.f), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 T = glm::translate(glm::vec3(10.f, 10.f, 10.f));
	glm::mat4 S = glm::scale(glm::vec3(1.f, 2.f, 1.f));
	
	// composition
	glm::mat4 M = T * R * S;

	// transform
	glm::vec4 p0_t = M * p0;
	glm::vec4 p1_t = M * p1;
	glm::vec4 u0_nt = M * u0_nt;
	glm::vec4 u0_ntn = glm::normalize(u0_nt);

	// Print out
	std::cout << "p0 = " << glm::to_string(p0) << std::endl << std::endl;
	std::cout << "p1 = " << glm::to_string(p1) << std::endl << std::endl;
	std::cout << "u0 = " << glm::to_string(u0) << std::endl << std::endl;
	std::cout << "u0_n = " << glm::to_string(u0_n) << std::endl << std::endl;
	std::cout << "R = " << glm::to_string(R) << std::endl << std::endl;
	std::cout << "T = " << glm::to_string(T) << std::endl << std::endl;
	std::cout << "S = " << glm::to_string(S) << std::endl << std::endl;
	std::cout << "M = TRS = " << glm::to_string(M) << std::endl << std::endl;
	std::cout << "M*p0 = " << glm::to_string(p0_t) << std::endl << std::endl;
	std::cout << "M*p1 = " << glm::to_string(p1_t) << std::endl << std::endl;
	std::cout << "M*u0_n = " << glm::to_string(u0_n) << std::endl << std::endl;
	std::cout << "nomalize(M*u0_n) = " << glm::to_string(u0_ntn) << std::endl << std::endl;

	return 0;
}




