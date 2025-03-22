#ifndef FIRE_H
#define FIRE_H

#include "SceneObject.h"

class Fire : public SceneObject {
public:
	Fire();
	virtual ~Fire();

	void Update(float deltaTime) override;

	void sendSpecificData() override;

	float u_DispMax;
	float u_Time;
};

#endif // !FIRE_H


