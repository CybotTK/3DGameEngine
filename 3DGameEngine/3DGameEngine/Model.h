#ifndef MODEL_H
#define MODEL_H

#include "../Utilities/utilities.h"
#include <string>
#include <vector>

#include "Vertex.h"

struct ModelResource {
	std::string filePath; 
	int id;
};

class Model {
public:
	Model();
	virtual ~Model();

	void Load();

	void Set(); //TESTING TESTING

	GLuint GetVboId();
	GLuint GetEboId();
	GLuint GetWiredEboId();

	int GetNrIndices();
	int GetNrIndicesSquare();
	int GetNrIndicesWired();

	void GenerateSquareModel(int numberOfCells = 4, int cellSize = 5);

	int nrOfCells;
	int sizeOfCell;

	ModelResource* m_modelResource;

	//Local Coordinates System
	std::vector<Vertex> squareVerticesCoords;
	std::vector<unsigned short> squareIndicesArray;

	GLuint vboId; // buffer that contains vertexes

private:

	GLuint eboId; // buffer for indices
	GLuint wiredEboId; // buffer for indices for wired view

	int nrIndices, nrIndicesSquare;
	int nrIndicesWired;

	std::vector<Vertex> verticesCoords;
	std::vector<unsigned short> indicesArray;
	
};

#endif // !MODEL_H
