#include "stdafx.h"
#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Update(float deltaTime) {
	if (this->hasFollowingCamera) {
		if (this->followingCamera.x == 1) {
			this->position.x = this->m_camera->position.x;
		}
		if (this->followingCamera.z == 1) {
			this->position.z = this->m_camera->position.z;
		}
	}
}
