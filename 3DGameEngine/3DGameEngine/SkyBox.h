#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "SceneObject.h"
#include "Camera.h"

class SkyBox : public SceneObject {
public:
	SkyBox();
	virtual ~SkyBox();

	void Update(float deltaTime) override;
};

#endif // !SKY_BOX_H
