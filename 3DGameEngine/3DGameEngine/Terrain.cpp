#include "stdafx.h"
#include "Terrain.h"
#include "Model.h"
#include "Camera.h"
#include "Shaders.h"

Terrain::Terrain() {
	terrainTranslation.SetIdentity();
}

Terrain::~Terrain()
{
}

void Terrain::sendSpecificData() {

	if (this->shader->uv2Attribute != -1)
	{
		glEnableVertexAttribArray(this->shader->uv2Attribute);
		glVertexAttribPointer(this->shader->uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv2));
	}

	if (this->shader->heightUniform != -1) {
		glEnableVertexAttribArray(this->shader->heightUniform);
		glUniform3fv(this->shader->heightUniform, 1, &height.x);
	}

	Matrix mvp = this->modelMatrix * this->m_camera->viewMatrix * this->m_camera->perspectiveMatrix;

	mvp = terrainTranslation * mvp;

	if (this->shader->matrixUniform != -1)
	{
		glUniformMatrix4fv(this->shader->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}

	this->countIndices = model->GetNrIndicesSquare();
}

void Terrain::Update(float deltaTime) {
	TerrainUpdate(this->m_camera->position);
}


void Terrain::TerrainUpdate(Vector3 cameraPosition) {

	float dx = abs(cameraPosition.x - terrainCenter.x);
	float dz = abs(cameraPosition.z - terrainCenter.z);

	bool changesMade = false;

	if (dx > this->model->sizeOfCell) {
		float uvOffsetX = 1.0 / this->model->nrOfCells;

		if (cameraPosition.x > terrainCenter.x) {
			terrainCenter.x += this->model->sizeOfCell; // Moving terrain to the right
			for (auto& vertex : this->model->squareVerticesCoords) {
				vertex.uv2.y += uvOffsetX;
			}
		}
		else {
			terrainCenter.x -= this->model->sizeOfCell; // Moving terrain to the left
			for (auto& vertex : this->model->squareVerticesCoords) {
				vertex.uv2.y -= uvOffsetX;
			}
		}

		terrainTranslation.SetTranslation(terrainCenter);
	}

	if (dz > this->model->sizeOfCell) {
		float uvOffsetZ = 1.0 / this->model->nrOfCells;

		if (cameraPosition.z > terrainCenter.z) {
			terrainCenter.z += this->model->sizeOfCell; // Moving terrain to the front
			for (auto& vertex : this->model->squareVerticesCoords) {
				vertex.uv2.x += uvOffsetZ;
			}
		}
		else {
			terrainCenter.z -= this->model->sizeOfCell; // Moving terrain to the back
			for (auto& vertex : this->model->squareVerticesCoords) {
				vertex.uv2.x -= uvOffsetZ;
			}
		}

		terrainTranslation.SetTranslation(terrainCenter);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
	glBufferData(GL_ARRAY_BUFFER, this->model->squareVerticesCoords.size() * sizeof(Vertex), &this->model->squareVerticesCoords[0], GL_STATIC_DRAW);


}

void Terrain::TerrainUpdateUVs(Vector3 cameraPosition) {

	float uvOffsetX = (cameraPosition.x - terrainCenter.x) / this->model->nrOfCells;
	float uvOffsetZ = (cameraPosition.z - terrainCenter.z) / this->model->nrOfCells;

	terrainTranslation.SetTranslation(terrainCenter);

	std::cout <<uvOffsetX <<" "<<uvOffsetZ<<'\n';

	for (auto& vertex : this->model->squareVerticesCoords) {
		vertex.uv2.x -= uvOffsetX;
		vertex.uv2.y -= uvOffsetZ;
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
	glBufferData(GL_ARRAY_BUFFER, this->model->squareVerticesCoords.size() * sizeof(Vertex), &this->model->squareVerticesCoords[0], GL_STATIC_DRAW);

}
