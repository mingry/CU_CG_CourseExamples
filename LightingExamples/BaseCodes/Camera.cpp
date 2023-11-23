


#include "Camera.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>


static glm::vec3 rotate(glm::quat q, glm::vec3 v)
{
	glm::mat3x3 m;
	//cml::matrix_rotation_quaternion(m, q);
	m = glm::toMat3(q);

	return m*v;//cml::transform_vector(m, v);
}

static const glm::vec3 x_axis(1, 0, 0);
static const glm::vec3 y_axis(0, 1, 0);
static const glm::vec3 z_axis(0, 0, 1);

// camera가 초기 상태일때 z축의 양의 방향으로 보고 있게 하기 위해서 사용된다.
static glm::quat y180((float)cos(glm::pi<float>()/2), 0.f, (float)sin(glm::pi<float>()/2), 0.f);

Camera::Camera()
{
	cameraZoom = glm::vec3(1, 1, 1);
	cameraP = glm::vec3(0, 0, 0);
	glm::quat q(1, 0, 0, 0);
	setRotation(q);
	
	
	fov = 45;
	aspect = 1;
	_near = 0.1f;
	_far = 10000.0f;

	trackBallR = 0.8f;

	flag_ortho = false;
	setOrthVolume(glm::vec3(200, 200, 200));

	pivot = glm::vec3(0, 0, 0);
}



void Camera::setGLMatrix(float m[16])
{
}

void Camera::getGLMatrix(float m[16])
{

}




void Camera::setTranslation(const glm::vec3 &v)
{
	cameraP = v;
}




void Camera::setRotation(const glm::quat &r)
{
	cameraQ = r;
//	cameraQ_y180 = quater(cos(M_PI/2), 0, sin(M_PI/2), 0) * r;
}

void Camera::setZoom(float z)
{
	cameraZoom.x = z;
	cameraZoom.y = z;
	cameraZoom.z = z;
}


void Camera::setZoom(glm::vec3 z)
{
	cameraZoom = z;
}

void Camera::setZoom(float sx, float sy, float sz)
{
	setZoom(glm::vec3(sx, sy, sz));
}




void Camera::setFov(float f)
{
	fov = f;
}

float Camera::getFov() const
{
	if ( flag_ortho ) return 0;
	return fov;
}




void Camera::setAspectRatio(float r)
{
	aspect = r;
}

float Camera::getAspectRatio() const
{
	return aspect;
}

void Camera::setNearFar(float n, float f)
{
	_near = n;
	_far = f;
}

float Camera::getNear() const
{
	if ( flag_ortho ) return orth_viewport_volume_min[2];
	return _near;
}

float Camera::getFar() const
{
	if ( flag_ortho ) return orth_viewport_volume_max[2];
	return _far;
}



glm::vec3 Camera::getTranslation() const
{
	return cameraP;
}

glm::quat Camera::getRotation() const
{
	return cameraQ;
}

glm::vec3 Camera::getZoom() const
{
	return cameraZoom;
}

glm::vec3 Camera::getTranslationForGL() const
{
	return -1.0f*cameraP;
}

glm::quat Camera::getRotationForGL() const
{
	return y180 * glm::inverse(cameraQ);
}


glm::vec3 Camera::projectToTrackBall(float x, float y)
{
	float d, t, z;
	float r = trackBallR;

	d = (float) sqrt(x*x + y*y);
	if( d < r * 0.70710678118654752440f ) // Inside sphere 
	{		
		z = (float)sqrt( r*r-d*d );
	} 
	else // On hyperbola
	{
		t = r / 1.41421356237309504880f;
		z = t*t / d;
	}
	return glm::vec3(x, y, z);
}



void Camera::inputMouse(int button, float x1, float y1, float x2, float y2, float speedScale)
{
	float t_scale = 700;
	//x1 = x2 = 0.5;

	if ( speedScale > 0 )
	{
		t_scale = t_scale * speedScale;
	}

	if ( button == IN_TRANS )
	{
		glm::vec3 t(0, 0, 0);
		t[0] = t_scale * (x2 - x1);
		t[1] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		float s;
		if ( speedScale > 0 ) 
			s = exp((y2 - y1)*speedScale);
		else
			s = exp(y2 - y1);

		cameraP[0] -= pivot[0]*cameraZoom[0] ;
		cameraP[1] -= pivot[1]*cameraZoom[1] ;
		cameraP[2] -= pivot[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += pivot[0]*cameraZoom[0] ;
		cameraP[1] += pivot[1]*cameraZoom[1] ;
		cameraP[2] += pivot[2]*cameraZoom[2] ;
	}
	else if ( button == IN_TRANS_Z )
	{
		glm::vec3 t(0, 0, 0);
		t[2] = -1 * t_scale * (y2 - y1);

		cameraP += rotate( cameraQ, t);
	}
	else if ( button == IN_ROTATION )
	{
		glm::vec3 p1 = projectToTrackBall( x1*2-1, y1*2-1 );
		glm::vec3 p2 = projectToTrackBall( x2*2-1, y2*2-1 );

		/*
		float tmpF = p1[0];
		p1[0] = p2[0];
		p2[0] = tmpF;

		p1[0] *= -1;
		p2[0] *= -1;
		*/
		
		// Figure out how much to rotate around that axis
		glm::vec3 d = p2 - p1;
		float dLen = d.length() / (2.0f * trackBallR);

		// Avoid problems with out-of-control values
		if ( dLen > 1.0f ) 
		{
			dLen = 1.0f;
		}
		else if ( dLen < -1.0f )
		{
			dLen = -1.0f;
		}

		float phi = 2.0f * (float)asin(dLen);

		glm::vec3 cross = glm::cross(p1, p2);
		cross[1] *= -1;
		glm::quat q = glm::angleAxis((float)phi, glm::normalize(cross));
		//cml::quaternion_rotation_axis_angle(q, normalize(cross), phi);


		glm::quat tmpQ = cameraQ*q*glm::inverse(cameraQ);
		tmpQ = glm::normalize(tmpQ);
		glm::vec3 s_pivot = pivot;
		s_pivot[0] *= cameraZoom[0];
		s_pivot[1] *= cameraZoom[1];
		s_pivot[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_pivot)+s_pivot;
		cameraQ = cameraQ*q;


		/*glm::quat tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		cameraP = rotate(tmpQ, cameraP);
		cameraQ = cameraQ*q;*/
	
	}
	else if ( button == IN_ROTATION_Y_UP )
	{
		float dy = y2-y1;
		float dx = x2-x1;

		glm::quat rot_x;// = exp(-1*dy*x_axis);
		// cml::quaternion_rotation_axis_angle(rot_x, x_axis, -1*dy*2);
		rot_x = glm::angleAxis((float)(-1*dy*2), glm::vec3(1, 0, 0));


		glm::quat rot_y;// = exp(-1*dx*y_axis);
		rot_y = glm::angleAxis((float)(-1*dx*2), glm::vec3(0, 1, 0));
		//cml::quaternion_rotation_axis_angle(rot_y, y_axis, -1*dx*2);


		glm::quat q = rot_y * rot_x;

		glm::quat tmpQ = rot_y*cameraQ*rot_x*glm::inverse(cameraQ);
		tmpQ = normalize(tmpQ);
		glm::vec3 s_pivot = pivot;
		s_pivot[0] *= cameraZoom[0];
		s_pivot[1] *= cameraZoom[1];
		s_pivot[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_pivot)+s_pivot;
		cameraQ = rot_y*cameraQ*rot_x;


		/*glm::quat tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		cameraP = rotate(tmpQ, cameraP);
		cameraQ = cameraQ*q;*/
	
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + (y2 - y1)*10);
	}

	
}

void Camera::inputMouse(int button, float x1, float y1, float x2, float y2, glm::vec3 center, float speedScale)
{
	float t_scale = 700;
	//x1 = x2 = 0.5;

	if ( speedScale > 0 )
	{
		t_scale = t_scale * speedScale;
	}

	if ( button == IN_TRANS )
	{
		glm::vec3 t(0, 0, 0);
		t[0] = t_scale * (x2 - x1);
		t[1] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		float s;
		if ( speedScale > 0 ) 
			s = exp((y2 - y1)*speedScale);
		else
			s = exp(y2 - y1);

		cameraP[0] -= center[0]*cameraZoom[0] ;
		cameraP[1] -= center[1]*cameraZoom[1] ;
		cameraP[2] -= center[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += center[0]*cameraZoom[0] ;
		cameraP[1] += center[1]*cameraZoom[1] ;
		cameraP[2] += center[2]*cameraZoom[2] ;
	}
	else if ( button == IN_TRANS_Z )
	{
		glm::vec3 t(0, 0, 0);
		t[2] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ROTATION )
	{
		glm::vec3 p1 = projectToTrackBall( x1*2-1, y1*2-1 );
		glm::vec3 p2 = projectToTrackBall( x2*2-1, y2*2-1 );

		/*
		float tmpF = p1[0];
		p1[0] = p2[0];
		p2[0] = tmpF;

		p1[0] *= -1;
		p2[0] *= -1;
		*/
		
		// Figure out how much to rotate around that axis
		glm::vec3 d = p2 - p1;
		float dLen = d.length() / (2.0f * trackBallR);

		// Avoid problems with out-of-control values
		if ( dLen > 1.0f ) 
		{
			dLen = 1.0f;
		}
		else if ( dLen < -1.0f )
		{
			dLen = -1.0f;
		}

		float phi = 2.0f * (float)asin(dLen);

		glm::vec3 cross = glm::cross(p1, p2);
		cross[1] *= -1;
		glm::quat q;
		q = glm::angleAxis((float)phi, glm::normalize(cross));


		glm::quat tmpQ = cameraQ*q*glm::inverse(cameraQ);
		tmpQ = normalize(tmpQ);
		glm::vec3 s_center = center;
		s_center[0] *= cameraZoom[0];
		s_center[1] *= cameraZoom[1];
		s_center[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_center)+s_center;
		cameraQ = cameraQ*q;
		//cameraP = cml::length(cameraP) * rotate(cameraQ.inverse(), z_axis);
	
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + (y2 - y1)*10);
	}

	
}



void Camera::inputMouse(int button, int dx, int dy, float speedScale)
{
	if ( button == IN_TRANS )
	{
		glm::vec3 t(0, 0, 0);
		t[0] = speedScale * (dx);
		t[1] = -1 * speedScale * (dy);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_TRANS_Z )
	{
		float moveScale = 50;

		if ( speedScale > 0 )
		{
			moveScale *= speedScale;
		}

		glm::vec3 t(0, 0, 0);
		t[2] = -1 * moveScale * dy;
		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		float s;
		if ( speedScale > 0 ) 
			s = exp(((float)dy/10)*speedScale);
		else
			s = exp((float)dy/10);

		cameraP[0] -= pivot[0]*cameraZoom[0] ;
		cameraP[1] -= pivot[1]*cameraZoom[1] ;
		cameraP[2] -= pivot[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += pivot[0]*cameraZoom[0] ;
		cameraP[1] += pivot[1]*cameraZoom[1] ;
		cameraP[2] += pivot[2]*cameraZoom[2] ;
	}
	else if ( button == IN_NONUNIFORM_ZOOM )
	{
		if ( speedScale > 0 ) 
		{
			cameraZoom[0] *= exp(((float)dx/10)*speedScale);
			cameraZoom[1] *= exp(((float)dy/10)*speedScale);
			cameraZoom[2] *= exp(((float)dx/10)*speedScale);
		}
		else
		{
			cameraZoom[0] *= exp((float)dx/10);
			cameraZoom[1] *= exp((float)dy/10);
			cameraZoom[2] *= exp((float)dx/10);
		}
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + dy);
	}
}	


void Camera::inputMouse(int button, int dx, int dy, int dz, float speedScale)
{
	if ( button == IN_NONUNIFORM_ZOOM )
	{
		if ( speedScale > 0 ) 
		{
			cameraZoom[0] *= exp(((float)dx/10)*speedScale);
			cameraZoom[1] *= exp(((float)dy/10)*speedScale);
			cameraZoom[2] *= exp(((float)dz/10)*speedScale);
		}
		else
		{
			cameraZoom[0] *= exp((float)dx/10);
			cameraZoom[1] *= exp((float)dy/10);
			cameraZoom[2] *= exp((float)dz/10);
		}
	}
	else 
	{
		inputMouse(button, dx, dy, speedScale);
	}
}	






void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	cameraP = eye;

	glm::vec3 f = glm::normalize(center - eye);
	glm::vec3 s = glm::normalize(glm::cross(glm::normalize(up), f));
	glm::vec3 u  = glm::normalize(glm::cross(f, s));

	glm::mat3x3 rm( s[0], s[1], s[2],
			u[0], u[1], u[2],
			f[0], f[1], f[2]);

	
	glm::quat rq;
	rq = glm::toQuat(rm);

	cameraQ = rq;

	
}

glm::mat4 Camera::GetGLProjectionMatrix() const
{
	glm::mat4 m;
	if ( flag_ortho )
	{
		//glOrtho((orth_viewport_volume_min[0]), (orth_viewport_volume_max[0]), 
		//	(orth_viewport_volume_min[1]), (orth_viewport_volume_max[1]), 
		//	(orth_viewport_volume_min[2]), (orth_viewport_volume_max[2]));
		// glm::matrix_orthographic_RH(m, (double)(orth_viewport_volume_min[0]), (double)(orth_viewport_volume_max[0]), 
		//	(double)(orth_viewport_volume_min[1]), (double)(orth_viewport_volume_max[1]), 
		//	(double)(orth_viewport_volume_min[2]), (double)(orth_viewport_volume_max[2]), cml::z_clip_neg_one);

		m = glm::ortho((float)(orth_viewport_volume_min[0]), (float)(orth_viewport_volume_max[0]), 
			(float)(orth_viewport_volume_min[1]), (float)(orth_viewport_volume_max[1]), 
			(float)(orth_viewport_volume_min[2]), (float)(orth_viewport_volume_max[2]));
	}
	else
	{
		//gluPerspective(getFov(), getAspectRatio(), _near, _far);
		// cml::matrix_perspective_yfov_RH(m, (double)getFov(), (double)getAspectRatio(), (double)_near, (double)_far, cml::z_clip_neg_one);
		//cml::matrix_perspective_yfov_RH(m, (float)cml::rad(30.0f), (float)aspect, 1.0f, 1000.0f, cml::z_clip_neg_one);
		m = glm::perspective((float)getFov(), (float)getAspectRatio(), (float)_near, (float)_far);
	}

	return m;
}

glm::mat4 Camera::GetGLViewMatrix() const
{
	glm::mat4 m;
	

	// camera가 초기 상태일때 z축의 양의 방향으로 보고 있게 하기 위해 항성 y180을 먼저 적용한다.
	//cml::vector3d qv =  
	float angle;
	glm::vec3 axis;
	angle = glm::angle( y180 * inverse(cameraQ) );
	axis = glm::axis( y180 * inverse(cameraQ) );
	
	//cml::quaternion_to_axis_angle( y180 * inverse(cameraQ), axis, angle );
	// glRotatef(180.0*angle/cml::constantsd::pi(), axis[0], axis[1], axis[2]);
	m = glm::rotate((float)angle, axis);

	glm::vec3 t = -1.f * cameraP;
	//glTranslatef(t[0], t[1], t[2]);
	m = m*glm::translate(t);

	//glScalef(cameraZoom[0], cameraZoom[1], cameraZoom[2]);
	m =  m*glm::scale(cameraZoom);

	return m;
}


glm::mat4 Camera::GetGLViewMatrixInverse() const
{
	return glm::inverse(GetGLViewMatrix());

}



