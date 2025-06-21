#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "../Utilities/utilities.h"
#include <vector>

#include "SceneObject.h"

class Trajectory {
public:
	Trajectory();
	virtual ~Trajectory();

	Vector3 startPoint;	// will most of the time if not always be
						// the position of the object
						// center also
	Vector3 currentPos;	// will keep track where inbetween points
						// the object is
	Vector3 nextPoint;	// next point in the trajectory
						// get it from points

	std::vector<Vector3> points;
	int idxPoint = 0;
	int nrPoints;

	enum Type {
		LINEAR,
		LINE_STRIP,
		LINE_LOOP,
		CIRCLE,
	}type;

	enum Direction {
		NORMAL,
		BACK_FORTH,
	}direction;

	int iterationCount = 1; // -1 means infinite
	float speed = 1.f;

	float radius = 1.f;

	bool alternate = false;

	void Update(float deltaTime);

	Matrix lerpTranslation;
private:
	void UpdateNormalTrajectory(float deltaTime);
	void UpdateAlternateTrajectory(float deltaTime);

	void UpdateLoopTrajectory(float deltaTime);

	void UpdateCircleTrajectory(float deltaTime);
};

#endif TRAJECTORY_H

