#include "stdafx.h"

#include "Model.h"

#include <fstream>
#include <vector>

Model::Model()
{
}

Model::~Model()
{
	delete m_modelResource;
	verticesCoords.clear();
	indicesArray.clear();
}

void Model::Load()
{
	std::ifstream f;
	f.open(m_modelResource->filePath.c_str());
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " << m_modelResource->filePath << std::endl;
		return;
	}
	char buffer[256];
	f.getline(buffer, 256, ' '); // gets first word
	f.getline(buffer, 256);		 // gets number of vertices
	int countVertices = atoi(buffer);

	GLfloat xCoord, yCoord, zCoord;

	//Vertices
	for (int count = 0; count < countVertices; count++)
	{

#pragma region pos

		f.getline(buffer, 256, '[');
		f.getline(buffer, 256, ',');
		xCoord = atof(buffer);
		f.getline(buffer, 256, ',');
		yCoord = atof(buffer);
		f.getline(buffer, 256, ']');
		zCoord = atof(buffer);

		Vector3 posCoords(xCoord, yCoord, zCoord);

#pragma endregion

#pragma region norm

		f.getline(buffer, 256, '[');
		f.getline(buffer, 256, ',');
		xCoord = atof(buffer);
		f.getline(buffer, 256, ',');
		yCoord = atof(buffer);
		f.getline(buffer, 256, ']');
		zCoord = atof(buffer);

		Vector3 normCoords(xCoord, yCoord, zCoord);

#pragma endregion

#pragma region binorm

		f.getline(buffer, 256, '[');
		f.getline(buffer, 256, ',');
		xCoord = atof(buffer);
		f.getline(buffer, 256, ',');
		yCoord = atof(buffer);
		f.getline(buffer, 256, ']');
		zCoord = atof(buffer);

		Vector3 binormCoords(xCoord, yCoord, zCoord);

#pragma endregion

#pragma region tgt

		f.getline(buffer, 256, '[');
		f.getline(buffer, 256, ',');
		xCoord = atof(buffer);
		f.getline(buffer, 256, ',');
		yCoord = atof(buffer);
		f.getline(buffer, 256, ']');
		zCoord = atof(buffer);

		Vector3 tgtCoords(xCoord, yCoord, zCoord);

#pragma endregion

#pragma region uv

		f.getline(buffer, 256, '[');
		f.getline(buffer, 256, ',');
		xCoord = atof(buffer);
		f.getline(buffer, 256, ']');
		yCoord = atof(buffer);

		Vector2 uvCoords(xCoord, yCoord);

#pragma endregion

		Vector3 colorVal(0.f, 0.f, 0.f);

		verticesCoords.push_back({ posCoords, colorVal, normCoords, binormCoords, tgtCoords, uvCoords, Vector2()});

	}

	f.getline(buffer, 256, ' '); // gets first word
	f.getline(buffer, 256);		 // gets number of indices
	nrIndices = atoi(buffer); // ATM just have it global

#pragma region Indices

	unsigned short firstIndex, sndIndex, thirdIndex;
	for (int count = 0; count < nrIndices; count++)
	{
		f.getline(buffer, 256, '.'); // index
		f.getline(buffer, 256, ','); // first
		firstIndex = atoi(buffer);
		f.getline(buffer, 256, ','); // second
		sndIndex = atoi(buffer);
		f.getline(buffer, 256, '\n'); // third
		thirdIndex = atoi(buffer);

		indicesArray.push_back(firstIndex);
		indicesArray.push_back(sndIndex);
		indicesArray.push_back(thirdIndex);
	}

#pragma endregion

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verticesCoords.size() * sizeof(Vertex), &verticesCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesArray.size() * sizeof(unsigned short), indicesArray.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Set() //TESTING TESTING
{
	m_modelResource = new ModelResource;

	m_modelResource->id = 1;
	m_modelResource->filePath = "C:/GameloftWorkshop/proiect_2015/OutsideResources/Models/Croco.nfg";
}

int Model::GetNrIndices()
{
	return nrIndices;
}

int Model::GetNrIndicesSquare()
{
	return nrIndicesSquare;
}

int Model::GetNrIndicesWired()
{
	return nrIndicesWired;
}

void Model::GenerateSquareModel(int numberOfCells, int cellSize)
{
	nrOfCells = numberOfCells;
	sizeOfCell = cellSize;

	float start = -numberOfCells / 2.0f * cellSize; 
	float end = numberOfCells / 2.0f * cellSize;    
	float z = -10.0f;            

	for (float x = start; x <= end; x += cellSize) {
		for (float y = start; y <= end; y += cellSize) {
			Vertex vertex;
			vertex.pos = Vector3(x, z, y);
			vertex.norm = Vector3(0, 0, 1);
			vertex.uv = Vector2(y, x);
			vertex.uv2 = Vector2((y - start) / (end - start), (x - start) / (end - start));

			squareVerticesCoords.push_back(vertex);
		}
	}

	for (int i = 0; i < numberOfCells; ++i) {
		for (int j = 0; j < numberOfCells; ++j) {
			// Indices for 1st triangle
			squareIndicesArray.push_back(i * (numberOfCells + 1) + j);
			squareIndicesArray.push_back((i + 1) * (numberOfCells + 1) + j);
			squareIndicesArray.push_back(i * (numberOfCells + 1) + (j + 1));

			// Indices for 2nd triangle
			squareIndicesArray.push_back(i * (numberOfCells + 1) + (j + 1));
			squareIndicesArray.push_back((i + 1) * (numberOfCells + 1) + j);
			squareIndicesArray.push_back((i + 1) * (numberOfCells + 1) + (j + 1));
		}
	}

	nrIndicesSquare = squareIndicesArray.size();

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, squareVerticesCoords.size() * sizeof(Vertex), &squareVerticesCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, squareIndicesArray.size() * sizeof(unsigned short), squareIndicesArray.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

GLuint Model::GetVboId()
{
	return vboId;
}

GLuint Model::GetEboId()
{
	return eboId;
}

GLuint Model::GetWiredEboId()
{
	return wiredEboId;
}
