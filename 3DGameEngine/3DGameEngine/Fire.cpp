#include "stdafx.h"
#include "Fire.h"
#include "Shaders.h"

Fire::Fire() {
}

Fire::~Fire() {
}

void Fire::Update(float deltaTime) {
	u_Time = static_cast<float>(clock() / 1000.f);
}

void Fire::sendSpecificData() {
	if (this->shader->dispMaxUniform != -1) {
		glUniform1f(this->shader->dispMaxUniform, u_DispMax);
	}

	if (this->shader->timeUniform != -1) {
		glUniform1f(this->shader->timeUniform, u_Time);
	}
}

