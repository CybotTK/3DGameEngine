#include "stdafx.h"

#include "SceneObject.h"
#include "SceneManager.h"

#include "Model.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{

	for (Texture* texture : textures) {
		delete texture;
	}
	textures.clear();

	delete model;
	delete shader;
	delete m_camera;
}

void SceneObject::sendCommonData()
{
	Matrix translationMatrix;
	Matrix rotationMatrix;
	Matrix rotationMatrixX, rotationMatrixY, rotationMatrixZ;
	Matrix scaleMatrix;

	translationMatrix.SetTranslation(position);

	rotationMatrixX.SetRotationX(rotation.x);
	rotationMatrixY.SetRotationX(rotation.y);
	rotationMatrixZ.SetRotationX(rotation.z);
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
		glUniform1f(shader->fogSmallRadiusUniform, SceneManager::GetInstance()->fogSmallRadius);
	}

	if (shader->fogBigRadiusUniform != -1) {
		glUniform1f(shader->fogBigRadiusUniform, SceneManager::GetInstance()->fogBigRadius);
	}

	if (shader->fogColorUniform != -1) {
		glUniform3f(shader->fogColorUniform, 0.235294f, 0.2666666f, 0.3333333f); // same color as horizon
	}

	if (!glIsBuffer(modelEboId)) {
		std::cerr << "No valid EBO bound!" << std::endl;
	}

#pragma endregion
}

void SceneObject::sendSpecificData() {
}

void SceneObject::Draw()
{
	if (enableDepthTest == true) {
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	else {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glUseProgram(shader->GetProgram());
	sendCommonData();
	sendSpecificData();

	glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_SHORT, 0);
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
}
