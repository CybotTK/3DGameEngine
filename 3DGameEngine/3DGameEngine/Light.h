#ifndef LIGHT_H
#define LIGHT_H

#include "../Utilities/utilities.h"

class Light {
public:
	Light();
	virtual ~Light();

	enum LightType {
		DIRECTIONAL,
		POINT,
		SPOTLIGHT
	} type;

	Vector3 diffuseColor;
	Vector3 specularColor;
	float specularPower = 1.0f;
	
	int associatedObjectId;

	Vector3 position;
	Vector3 direction; 
	float range = 0.0; //point light

	float outerAngle;
	float innerAngle;
private:
};

#endif // !LIGHT_H


