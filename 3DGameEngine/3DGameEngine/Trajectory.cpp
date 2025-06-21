#include "stdafx.h"
#include "Trajectory.h"
#include "Model.h"
#include "Camera.h"
#include "Shaders.h"

Trajectory::Trajectory() {
	lerpTranslation.SetTranslation(startPoint);
}

Trajectory::~Trajectory() {
}

void Trajectory::Update(float deltaTime) {
	if (type == LINEAR || type == LINE_STRIP) {
		if (direction == NORMAL) {
			UpdateNormalTrajectory(deltaTime);
		}
		else if (direction == BACK_FORTH) {
			UpdateAlternateTrajectory(deltaTime);
		}
	}
	else if (type == LINE_LOOP) {
		UpdateLoopTrajectory(deltaTime);
	}
	else if (type == CIRCLE) {
		UpdateCircleTrajectory(deltaTime);
	}
}

void Trajectory::UpdateNormalTrajectory(float deltaTime) {
	if (iterationCount == 0) {
		return;
	}

	if (idxPoint < nrPoints) {
		Lerp(currentPos, points[idxPoint], speed, deltaTime);
		lerpTranslation.SetTranslation(currentPos);
		if (currentPos == points[idxPoint]) {
			idxPoint++;
		}
	}
	else {
		if (iterationCount != -1) {
			iterationCount--;
		}
		idxPoint = 0;
		currentPos = startPoint;
		lerpTranslation.SetTranslation(currentPos);
	}
}

void Trajectory::UpdateAlternateTrajectory(float deltaTime) {
	if (iterationCount == 0) {
		return;
	}

	if (idxPoint < nrPoints && alternate == false) {
		Lerp(currentPos, points[idxPoint], speed, deltaTime);
		lerpTranslation.SetTranslation(currentPos);
		if (currentPos == points[idxPoint]) {
			idxPoint++;
		}
		if (idxPoint == nrPoints) {
			alternate = true;
			idxPoint--; // we go to the last point of the vector
		}
	}
	else { // alternate == true
		if (idxPoint > -1) {
			Lerp(currentPos, points[idxPoint], speed, deltaTime);
			lerpTranslation.SetTranslation(currentPos);
			if (currentPos == points[idxPoint]) {
				idxPoint--;
			}
		}
		else if (idxPoint == -1) { // We have to get to the start point
			Lerp(currentPos, startPoint, speed, deltaTime);
			lerpTranslation.SetTranslation(currentPos);
			if (currentPos == startPoint) {
				idxPoint = 0;
				alternate = false;
				if (iterationCount != -1) {
					iterationCount--;
				}
			}
		}
	}
}

void Trajectory::UpdateLoopTrajectory(float deltaTime) {
	if (iterationCount == 0) {
		return;
	}

	if (idxPoint < nrPoints) {
		Lerp(currentPos, points[idxPoint], speed, deltaTime);
		lerpTranslation.SetTranslation(currentPos);
		if (currentPos == points[idxPoint]) {
			idxPoint++;
		}
	}
	else {
		Lerp(currentPos, startPoint, speed, deltaTime);
		lerpTranslation.SetTranslation(currentPos);
		if (currentPos == startPoint) {
			if (iterationCount != -1) {
				iterationCount--;
			}
			idxPoint = 0;
		}
	}
	
}

void Trajectory::UpdateCircleTrajectory(float deltaTime) {
	static float angleDeg = 0.0f; // track angle of rotation

	if (iterationCount != -1 && iterationCount == 0) {
		return;
	}

	float angleIncrement = speed * deltaTime;
	angleDeg += angleIncrement;

	// Count full circles
	if (angleDeg >= 360.0f && iterationCount != -1) {
		angleDeg -= 360.0f;
		iterationCount--;
	}

	float theta = Radians(angleDeg); 
	// Circle point in XOZ plane (unit circle)
	Vector3 circleLocal = Vector3(cosf(theta) * radius, 0.0f, sinf(theta) * radius);

	// Convert rotation angles to radians
	float xRad = Radians(points[0].x);
	float yRad = Radians(points[0].y);
	float zRad = Radians(points[0].z);

	// Build rotation matrix (XYZ order)
	Matrix rotX, rotY, rotZ;
	rotX.SetRotationX(xRad);
	rotY.SetRotationY(yRad);
	rotZ.SetRotationZ(zRad);

	Matrix rotation = rotZ * rotY * rotX;

	// Apply rotation to circle vector
	Vector4 rotated = rotation * Vector4(circleLocal, 1.0f);
	currentPos = startPoint + Vector3(rotated.x, rotated.y, rotated.z);

	lerpTranslation.SetTranslation(currentPos);
}
