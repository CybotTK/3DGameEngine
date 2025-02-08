#include <stdafx.h>
#include "Shaders.h"


Shader::~Shader()
{
	glDeleteProgram(programId);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

int Shader::Load()
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, m_shaderResource->vsFilePath);

	if (vertexShader == 0)
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, m_shaderResource->fsFilePath);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	int error;

	programId = esLoadProgram(vertexShader, fragmentShader);
	error = glGetError();

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(programId, "a_posL");
	error = glGetError();
	colorAttribute = glGetAttribLocation(programId, "a_colorL");
	error = glGetError();
	uvAttribute = glGetAttribLocation(programId, "a_uv");
	error = glGetError();
	uv2Attribute = glGetAttribLocation(programId, "a_uv2");
	error = glGetError();
	
	modelMatrixUniform = glGetUniformLocation(programId, "uModelMatrix");
	error = glGetError();
	matrixUniform = glGetUniformLocation(programId, "uMVP");
	error = glGetError();

	for (int count = 0; count < MAX_TEXTURES; count++) {
		std::string uTexture = "u_texture" + std::to_string(count);
		const GLchar* uTexturePointer = uTexture.c_str();
		textureUniforms[count] = glGetUniformLocation(programId, uTexturePointer);
	}

	wireframeUniform = glGetUniformLocation(programId, "u_wireframe");
	error = glGetError();
	heightUniform = glGetUniformLocation(programId, "u_height");
	error = glGetError();

	cameraPositionUniform = glGetUniformLocation(programId, "uCameraPosition");
	error = glGetError();

	fogSmallRadiusUniform = glGetUniformLocation(programId, "u_fogSmallRadius");
	error = glGetError();
	fogBigRadiusUniform = glGetUniformLocation(programId, "u_fogBigRadius");
	error = glGetError();
	fogColorUniform = glGetUniformLocation(programId, "u_fogColor");
	error = glGetError();
	
	return 0;
}

void Shader::Set() //This is for testing
{
	m_shaderResource = new ShaderResource;

	//m_shaderResource->vsFilePath = "../Resources/Shaders/ModelShaderVS.vs";
	//m_shaderResource->fsFilePath = "../Resources/Shaders/ModelShaderFS.fs";
}

GLuint Shader::GetProgram()
{
	return programId;
}
