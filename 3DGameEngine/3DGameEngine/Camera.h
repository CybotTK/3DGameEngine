#ifndef CAMERA_H
#define CAMERA_H

#include <GLES2/gl2.h>
#include "../Utilities/Math.h"

class Camera {
public:

	Camera();
	Camera(Vector3 position, 
		Vector3 target, 
	 	Vector3 up,
	    GLfloat moveSpeed = 10.f,
		GLfloat rotateSpeed = 1.f,
		GLfloat nearPlane = 1.f,
		GLfloat farPlane = 1000.f,
		GLfloat fov = 90.f);
	~Camera();

	void MoveOx(int direction);
	void MoveOy(int direction);
	void MoveOz(int direction);
	void RotateOx(int angle);
	void RotateOy(int angle);
	void RotateOz(int angle);

	void UpdateAxes();
	void UpdateWorldView();
	void SetPerspectiveMatrix(float aspectRatio);
	void setDeltaTime(GLfloat time); 
	
	Matrix viewMatrix;
	Matrix worldMatrix;

	Matrix perspectiveMatrix;

	Vector3 position;
	Vector3 target;
	Vector3 up;

	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat fov;
	GLfloat deltaTime;

	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

private:

	
	//localUp and localTarget are local variables in camera system
};

#endif //CAMERA_H
