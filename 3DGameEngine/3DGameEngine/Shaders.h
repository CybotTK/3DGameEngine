#pragma once
#include "../Utilities/utilities.h"

struct ShaderResource {
	int id;
	char vsFilePath[256];
	char fsFilePath[256];
};

#define MAX_TEXTURES 6

class Shader 
{
public:
	GLuint vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	GLint uv2Attribute;

	GLint modelMatrixUniform;
	GLint matrixUniform;
	GLint textureUniforms[MAX_TEXTURES];
	GLint wireframeUniform;

	GLint cameraPositionUniform;

	GLint heightUniform;

	GLint fogSmallRadiusUniform;
	GLint fogBigRadiusUniform;
	GLint fogColorUniform;

	~Shader();

	int Load();

	void Set(); //TESTING TESTING

	GLuint GetProgram();

	ShaderResource* m_shaderResource;

private:

	GLuint programId; //what we get when linking vs and fs
};