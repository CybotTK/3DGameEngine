#include "stdafx.h"
#include "Camera.h"
#include "Globals.h"

Camera::Camera()
{
	position = Vector3(0.f, 0.f, -200.f);
	target = Vector3(0.f, 0.f, 0.f);
	up = Vector3(0.f, 1.f, 0.f);
	fov = Radians(90);
	nearPlane = 10.f;
	farPlane = 1000.f;
	moveSpeed = 100.f;
	rotateSpeed = 5.f;

	GLfloat aspectRatio = (float)Globals::screenWidth / (float)Globals::screenHeight;
	perspectiveMatrix = perspectiveMatrix.SetPerspective(fov, aspectRatio, nearPlane, farPlane);

	UpdateWorldView();
}

Camera::Camera(Vector3 positionNew, 
	Vector3 targetNew, 
	Vector3 upNew, 
	GLfloat moveSpeedNew, 
	GLfloat rotateSpeedNew, 
	GLfloat nearPlaneNew, 
	GLfloat farPlaneNew, 
	GLfloat fovNew) {
	position = positionNew;
	target = targetNew;
	up = upNew;
	moveSpeed = moveSpeedNew;
	rotateSpeed = rotateSpeedNew;
	nearPlane = nearPlaneNew;
	farPlane = farPlaneNew;
	fov = fovNew;

	GLfloat aspectRatio = (float)Globals::screenWidth / (float)Globals::screenHeight;
	perspectiveMatrix = perspectiveMatrix.SetPerspective(fov, aspectRatio, nearPlane, farPlane);
	
	UpdateWorldView();
}

Camera::~Camera()
{

}

void Camera::MoveOx(int direction)
{
	Vector3 forward = xAxis * direction;
	Vector3 vectorDisplacement = forward * moveSpeed * deltaTime;

	position += vectorDisplacement;
	target += vectorDisplacement;

	UpdateWorldView();
}

void Camera::MoveOy(int direction)
{
	Vector3 forward = yAxis * direction;
	Vector3 vectorDisplacement = forward * moveSpeed * deltaTime;

	position += vectorDisplacement;
	target += vectorDisplacement;

	UpdateWorldView();
}

void Camera::MoveOz(int direction)
{
	Vector3 forward = zAxis * direction;
	Vector3 vectorDisplacement = forward * moveSpeed * deltaTime;

	position += vectorDisplacement;
	target += vectorDisplacement;

	UpdateWorldView();
}

void Camera::RotateOx(int angle)
{
	Matrix mRotateOx;
	mRotateOx.SetRotationX(rotateSpeed * deltaTime * angle);
	
	Vector4 localUp = Vector4(0.f, 1.f, 0.f, 0.f);
	Vector4 rotatedLocalUp = localUp * mRotateOx;
	Vector4 upVec4;
	upVec4 = rotatedLocalUp * worldMatrix;
	up = ToVector3(upVec4);
	up.Normalize();

	Vector4 localTarget = Vector4(0.f, 0.f, -(target - position).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * mRotateOx;
	Vector4 targetVec4 = ToVector4(target);
	targetVec4 = rotatedTarget * worldMatrix;
	target = ToVector3(targetVec4);

	UpdateWorldView();

}

void Camera::RotateOy(int angle)
{
	Matrix mRotateOy;
	mRotateOy.SetRotationY(rotateSpeed * deltaTime * angle);

	Vector4 localTarget = Vector4(0.f, 0.f, -(target - position).Length(), 1.f);
	
	Vector4 rotatedTarget = localTarget * mRotateOy;

	Vector4 targetVec4;
	targetVec4 = rotatedTarget * worldMatrix;
	target = ToVector3(targetVec4);

	UpdateWorldView();
}

void Camera::RotateOz(int angle)
{
	Matrix mRotateOz;
	mRotateOz.SetRotationZ(rotateSpeed * deltaTime * angle);

	Vector4 localUp = Vector4(0.f, 1.f, 0.f, 0.f);
	Vector4 rotatedLocalUp = localUp * mRotateOz;
	Vector4 upVec4;
	upVec4 = rotatedLocalUp * worldMatrix;
	up = ToVector3(upVec4);
	up.Normalize();

	Vector4 localTarget = Vector4(0.f, 0.f, -(target - position).Length(), 1.f);
	Vector4 rotatedTarget = localTarget * mRotateOz;
	Vector4 targetVec4;
	targetVec4 = rotatedTarget * worldMatrix;
	target = ToVector3(targetVec4);

	UpdateWorldView();
}


void Camera::UpdateAxes()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::UpdateWorldView()
{
	UpdateAxes();
	Matrix t; //T - translation matrix for position vector
	t.SetTranslation(position);

	Matrix r; //R - rotation matrix
	r.m[0][0] = xAxis.x;
	r.m[0][1] = xAxis.y;
	r.m[0][2] = xAxis.z;
	r.m[0][3] = 0;
	r.m[1][0] = yAxis.x;
	r.m[1][1] = yAxis.y;
	r.m[1][2] = yAxis.z;
	r.m[1][3] = 0;
	r.m[2][0] = zAxis.x;
	r.m[2][1] = zAxis.y;
	r.m[2][2] = zAxis.z;
	r.m[2][3] = 0;
	r.m[3][0] = 0;
	r.m[3][1] = 0;
	r.m[3][2] = 0;
	r.m[3][3] = 1.f;

	worldMatrix = r * t;

	Matrix rTranspose = r.Transpose(); //inverse of r is r transpose since r is orthogonal	
	Matrix tInverse;
	tInverse.SetTranslation(-position);

	viewMatrix = tInverse * rTranspose; // worldMatrix inverse
}

void Camera::SetPerspectiveMatrix(float aspectRatio)
{
	perspectiveMatrix = perspectiveMatrix.SetPerspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::setDeltaTime(GLfloat time)
{
	deltaTime = time;
}
