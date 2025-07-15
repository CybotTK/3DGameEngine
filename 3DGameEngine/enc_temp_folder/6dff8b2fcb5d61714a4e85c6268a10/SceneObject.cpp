#include "stdafx.h"

#include "SceneObject.h"
#include "SceneManager.h"

#include "Model.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"
#include "Trajectory.h"

SceneObject::SceneObject() {
	m_sceneManagerInst = SceneManager::GetInstance();
}

SceneObject::~SceneObject() {
	delete m_sceneManagerInst;

	delete model;
	delete shader;

	for (Texture* texture : textures) {
		delete texture;
	}
	textures.clear();

	delete m_camera;

	delete reflectionTexture;

	delete traj;
}

void SceneObject::Init() {
	if (hasTrajectory == true) {
		position = traj->startPoint;
	}
}

void SceneObject::sendCommonData()
{
	objectName;

	Matrix translationMatrix;
	Matrix rotationMatrix;
	Matrix rotationMatrixX, rotationMatrixY, rotationMatrixZ;
	Matrix scaleMatrix;

	translationMatrix.SetTranslation(position);

	rotationMatrixX.SetRotationX(Radians(rotation.x));
	rotationMatrixY.SetRotationX(Radians(rotation.y));
	rotationMatrixZ.SetRotationX(Radians(rotation.z));
	rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

	scaleMatrix.SetScale(scale);

	modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	
	Matrix mvp = modelMatrix * m_camera->viewMatrix * m_camera->perspectiveMatrix;

#pragma region Model

	modelVboId = model->GetVboId();
	modelEboId = model->GetEboId();
	countIndices = model->GetNrIndices();

	int initialTextureFlag = 0x84C0;
	int textureFlag = 0x84C0;

	if (!textures.empty()) {
		for (auto texture : textures) {
			modelTextureId = texture->GetTextureId();

			glActiveTexture(GL_TEXTURE0 + (textureFlag - initialTextureFlag));
			glBindTexture(texture->m_textureResource->type, modelTextureId);

			if (shader->textureUniforms[textureFlag-initialTextureFlag] != -1)
			{
				glUniform1i(shader->textureUniforms[textureFlag - initialTextureFlag], textureFlag - initialTextureFlag);
			}

			textureFlag++;

		}

	}
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelEboId);

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shader->normAttribute != -1)
	{
		glEnableVertexAttribArray(shader->normAttribute);
		glVertexAttribPointer(shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	}

	if (shader->modelMatrixUniform != -1) {
		glUniformMatrix4fv(shader->modelMatrixUniform, 1, GL_FALSE, (float*)modelMatrix.m);
	}

	if (shader->matrixUniform != -1) {
		glUniformMatrix4fv(shader->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}

	if (shader->cameraPositionUniform != -1) {
		glUniform3f(shader->cameraPositionUniform, m_camera->position.x, m_camera->position.y, m_camera->position.z);
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (shader->wireframeUniform != -1) {
		glUniform1i(shader->wireframeUniform, isWired);
	}

	if (shader->fogSmallRadiusUniform != -1) {
		glUniform1f(shader->fogSmallRadiusUniform, m_sceneManagerInst->fogSmallRadius);
	}

	if (shader->fogBigRadiusUniform != -1) {
		glUniform1f(shader->fogBigRadiusUniform, m_sceneManagerInst->fogBigRadius);
	}

	if (shader->fogColorUniform != -1) {
		glUniform3f(shader->fogColorUniform, 0.235294f, 0.2666666f, 0.3333333f); // same color as horizon
	}

	if (shader->reflectionUniform != -1) {
		glUniform1f(shader->reflectionUniform, reflection);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(reflectionTexture->m_textureResource->type, reflectionTexture->GetTextureId());

		glUniform1i(shader->textureUniforms[1], 1);
	}

	if (shader->ambiLightUniform != -1) {
		Vector3 ambiLight = m_sceneManagerInst->ambiLight;
		glUniform3f(shader->ambiLightUniform, ambiLight.x, ambiLight.y, ambiLight.z);
	}

	if (shader->ratioAmbiLightUniform != -1) {
		glUniform1f(shader->ratioAmbiLightUniform, m_sceneManagerInst->ratioAmbiLight);
	}

	if (shader->numLightsUniform != -1) {
		glUniform1i(shader->numLightsUniform, numLights);
	}

	if (shader->lightDirUniform != -1) {
		Vector3 lightsDirArr[10];
		for (int i = 0; i < numLights; i++) {
			lightsDirArr[i] = m_sceneManagerInst->lights[lightsId[i]]->direction;
		}
		glUniform3fv(shader->lightDirUniform, numLights, &lightsDirArr[0].x);
	}

	if (shader->lightPosUniform != -1) {
		Vector3 lightsPosArray[10];
		for (int i = 0; i < numLights; i++) {
			lightsPosArray[i] = m_sceneManagerInst->lights[lightsId[i]]->position;
		}
		glUniform3fv(shader->lightPosUniform, numLights, &lightsPosArray[0].x);
	}

	if (shader->colorLightDiffUniform != -1) {
		Vector3 colorLightDiffArr[10];
		for (int i = 0; i < numLights; i++) {
			colorLightDiffArr[i] = m_sceneManagerInst->lights[lightsId[i]]->diffuseColor;
		}
		glUniform3fv(shader->colorLightDiffUniform, numLights, &colorLightDiffArr[0].x);
	}

	if (shader->colorLightSpecUniform != -1) {
		Vector3 colorLightSpecArr[10];
		for (int i = 0; i < numLights; i++) {
			colorLightSpecArr[i] = m_sceneManagerInst->lights[lightsId[i]]->specularColor;
		}
		glUniform3fv(shader->colorLightSpecUniform, numLights, &colorLightSpecArr[0].x);
	}
	
	if (shader->specPowerUniform != -1) {
		float specPowArr[10];
		for (int i = 0; i < numLights; i++) {
			specPowArr[i] = m_sceneManagerInst->lights[lightsId[i]]->specularPower;
		}
		glUniform1fv(shader->specPowerUniform, numLights, &specPowArr[0]);
	}

	if (shader->lightTypeUniform != -1) {
		int lightType[10];
		auto lightsArray = m_sceneManagerInst->lights;
		for (int i = 0; i < numLights; i++) {
			if (lightsArray[lightsId[i]]->type == Light::LightType::DIRECTIONAL) {
				lightType[i] = 0;
			}
			else if (lightsArray[lightsId[i]]->type == Light::LightType::POINT) {
				lightType[i] = 1;
			}
			else {
				lightType[i] = 2;
			}
		}
		glUniform1iv(shader->lightTypeUniform, numLights, &lightType[0]);
	}

	if (shader->lightRangeUniform != -1) {
		float rangeArray[10];
		for (int i = 0; i < numLights; i++) {
			rangeArray[i] = m_sceneManagerInst->lights[lightsId[i]]->range;
		}
		glUniform1fv(shader->lightRangeUniform, numLights, &rangeArray[0]);
	}

	if (shader->outerAngleUniform != -1) {
		float outerAngleArr[10];
		auto lightsArray = m_sceneManagerInst->lights;
		for (int i = 0; i < numLights; i++) {
			outerAngleArr[i] = Radians(lightsArray[lightsId[i]]->outerAngle);
		}
		glUniform1fv(shader->outerAngleUniform, numLights, &outerAngleArr[0]);
	}

	if (shader->innerAngleUniform != -1) {
		float innerAngleArr[10];
		auto lightsArray = m_sceneManagerInst->lights;
		for (int i = 0; i < numLights; i++) {
			innerAngleArr[i] = Radians(lightsArray[lightsId[i]]->innerAngle);
		}
		glUniform1fv(shader->outerAngleUniform, numLights, &innerAngleArr[0]);
	}

	if (shader->kDiffUniform != -1) {
		glUniform1f(shader->kDiffUniform, kDiff);
	}

	if (shader->kSpecUniform != -1) {
		glUniform1f(shader->kSpecUniform, kSpec);
	}

	if (!glIsBuffer(modelEboId)) {
		std::cerr << "No valid EBO bound!" << std::endl;
	}

#pragma endregion
}

void SceneObject::sendSpecificData() {
	if (hasTrajectory == true) {
		Matrix mvp = this->modelMatrix * this->m_camera->viewMatrix * this->m_camera->perspectiveMatrix;

		mvp = traj->lerpTranslation * mvp;

		if (this->shader->matrixUniform != -1) {
			glUniformMatrix4fv(this->shader->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
		}
	}
}

void SceneObject::Draw()
{
	if (enableDepthTest == true) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (enableBlend == true) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}

	glUseProgram(shader->GetProgram());
	sendCommonData();
	sendSpecificData();

	glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_SHORT, 0);
}

void SceneObject::DrawDebug() {
}

void SceneObject::DrawAxes() {
	glDisable(GL_DEPTH_TEST);

	int ok;

	glBindBuffer(GL_ARRAY_BUFFER, m_sceneManagerInst->cylinderVboId);
	ok = glGetError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sceneManagerInst->cylinderEboId);
	ok = glGetError();

	glUseProgram(m_sceneManagerInst->shaderAxes->GetProgram());

	// Disable color attribute array (we're using a constant color per axis)
	if (m_sceneManagerInst->shaderAxes->colorAttribute != -1)
		glDisableVertexAttribArray(m_sceneManagerInst->shaderAxes->colorAttribute);

	const auto& shaderAxes = m_sceneManagerInst->shaderAxes;
	const auto& cam = m_camera;

	Matrix baseTranslation;
	baseTranslation.SetTranslation(position);

	Matrix upTranslation;
	upTranslation.SetTranslation(Vector3(0, m_sceneManagerInst->cylinderHeight / 2, 0));

	Matrix mvp;

	auto drawAxis = [&](const Vector3& color, const Matrix& axisRotation) {
		mvp = upTranslation * axisRotation * baseTranslation * cam->viewMatrix * cam->perspectiveMatrix;

		if (shaderAxes->matrixUniform != -1) {
			glUniformMatrix4fv(shaderAxes->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
		}

		if (shaderAxes->positionAttribute != -1) {
			glEnableVertexAttribArray(shaderAxes->positionAttribute);
			glVertexAttribPointer(shaderAxes->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		}

		if (shaderAxes->colorAttribute != -1) {
			glVertexAttrib3f(shaderAxes->colorAttribute, color.x, color.y, color.z);
		}

		glDrawElements(GL_TRIANGLES, m_sceneManagerInst->axesIndices.size(), GL_UNSIGNED_SHORT, 0);
		};

	//===============ALSO ASK ABOUT THIS=======///

	// Draw Z axis (no rotation)
	drawAxis(m_sceneManagerInst->objectAxes.oZ, Matrix::Identity());

	// Draw Y axis (rotate around X)
	Matrix rotY;
	rotY.SetRotationX(Radians(90.f));
	drawAxis(m_sceneManagerInst->objectAxes.oY, rotY);

	// Draw X axis (rotate around Z)
	Matrix rotX;
	rotX.SetRotationZ(Radians(-90.f));
	drawAxis(m_sceneManagerInst->objectAxes.oX, rotX);
}

void SceneObject::Update(float deltaTime) {
	if (this->hasFollowingCamera) {
		if (this->followingCamera.x == 1) {
			this->position.x = this->m_camera->position.x + this->offsetCamera.x;
		}
		if (this->followingCamera.y == 1) {
			this->position.y = this->m_camera->position.y + this->offsetCamera.y;
		}
		if (this->followingCamera.z == 1) {
			this->position.z = this->m_camera->position.z + this->offsetCamera.z;
		}
	}
	if (hasTrajectory == true) {
		traj->Update(deltaTime);
	}
}