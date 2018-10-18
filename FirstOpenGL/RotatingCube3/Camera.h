
#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera
{
public:

	enum { IN_ROTATION, IN_ROTATION_Y_UP, IN_ZOOM, IN_TRANS, IN_TRANS_Z, IN_FOV, IN_NONUNIFORM_ZOOM };

	Camera();

	void setGLMatrix(double m[16]);
	void getGLMatrix(double m[16]);

	void setTranslation(const glm::vec3 &v);
	void setRotation(const glm::quat &q);
	void setZoom(double z);
	void setZoom(glm::vec3 z);
	void setZoom(double sx, double sy, double sz);

	void lookAt(glm::vec3 eye, glm::vec3 centor, glm::vec3 up);
	// void glTranslform() const;
	// void glTranslformInverse();
	// void glProjection() const;
	glm::mat4 GetGLViewMatrix() const;
	glm::mat4 GetGLViewMatrixInverse() const;
	glm::mat4 GetGLProjectionMatrix() const;

	void setFov(double f);
	double getFov() const;

	void setAspectRatio(double r);
	double getAspectRatio() const;

	void setOrthVolume(glm::vec3 volume) { orth_viewport_volume_min=volume*-0.5f; orth_viewport_volume_max=0.5f*volume; }
	void setOrthVolume(glm::vec3 volume_min, glm::vec3 volume_max) { orth_viewport_volume_min=volume_min; orth_viewport_volume_max=volume_max; }
	glm::vec3 getOrthVolumeSize() const { return orth_viewport_volume_max-orth_viewport_volume_min; }
	glm::vec3 getOrthVolumeMin() const { return orth_viewport_volume_min; }
	glm::vec3 getOrthVolumeMax() const { return orth_viewport_volume_max; }
	void setNearFar(double n, double f);
	double getNear() const;
	double getFar() const;

	glm::vec3 getTranslation() const;
	glm::quat getRotation() const;
	glm::vec3	getZoom() const;

	glm::vec3 getTranslationForGL() const;
	glm::quat getRotationForGL() const;

	// x1, y1 => previouse normalized mouse point, (0~1, 0~1).
	// x2, y2 => current normalized mouse point, (0~1, 0~1).
	void inputMouse(int button, double x1, double y1, double x2, double y2, double speedScale = -1.0f);
	void inputMouse(int button, double x1, double y1, double x2, double y2, glm::vec3 center, double speedScale = -1.0f);
	void setPivot(glm::vec3 p) { pivot = p; }


	// dx와 dy는 moust point 의 변화량. (y는 화면에서 높으수록 증가)
	void inputMouse(int button, int dx, int dy, double speedScale = -1.0f);

	void inputMouse(int button, int dx, int dy, int dz, double speedScale = -1.0f);

	void enableOrtho(bool f) { flag_ortho = f; }
	bool isOrtho() const { return flag_ortho; }


protected:
	glm::vec3 projectToTrackBall(double x, double y);

protected:
	glm::vec3 cameraP;

	// camera 의 실제 물리적 orientation
	glm::quat cameraQ;			
	
	//double cameraZoom;
	glm::vec3 cameraZoom;
	
	double fov;
	double aspect;
	bool flag_ortho;

	double _near, _far;

	double trackBallR;
	glm::vec3 orth_viewport_volume_min; 
	glm::vec3 orth_viewport_volume_max; 

	glm::vec3 pivot;
};




