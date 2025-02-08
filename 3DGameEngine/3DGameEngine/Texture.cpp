#include "stdafx.h"

#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Load()
{
	int error;

	pixelArray = LoadTGA(m_textureResource->filePath, &width, &height, &bpp);

	glGenTextures(1, &textureId);

	glBindTexture(m_textureResource->type, textureId);

	glTexParameteri(m_textureResource->type, GL_TEXTURE_WRAP_S, m_textureResource->wrapS);
	glTexParameteri(m_textureResource->type, GL_TEXTURE_WRAP_T, m_textureResource->wrapT);

	glTexParameteri(m_textureResource->type, GL_TEXTURE_MIN_FILTER, m_textureResource->minFilter);
	glTexParameteri(m_textureResource->type, GL_TEXTURE_MAG_FILTER, m_textureResource->magFilter);

	if (m_textureResource->type == GL_TEXTURE_2D)
	{
		if (bpp == 24)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);
		}
		if (bpp == 32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (m_textureResource->type == GL_TEXTURE_CUBE_MAP)
	{
		int collumnOffset[6] = {
			width / 2,
			0,
			width / 4,
			width / 4,
			width / 4,
			width * 3 / 4
		};

		int rowOffset[6] = {
			height / 3,
			height / 3,
			0,
			height * 2 / 3,
			height / 3,
			height / 3
		};

		int faceWidth = width / 4;
		int faceHeight = height / 3;

		if (bpp == 24 || bpp == 32) {
			GLenum format = (bpp == 24) ? GL_RGB : GL_RGBA;

			size_t faceSize = faceWidth * faceHeight * (bpp / 8);
			char* subBuffer = new char[faceSize];

			for (int face = 0; face < 6; ++face) {

				// We copy data from the big buffer
				for (int row = 0; row < faceHeight; ++row) {
					const char* sourceRow = pixelArray +
						(rowOffset[face] + row) * width * (bpp / 8) +
						collumnOffset[face] * (bpp / 8);

					char* destRow = subBuffer + row * faceWidth * (bpp / 8);
					memcpy(destRow, sourceRow, faceWidth * (bpp / 8));
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, subBuffer);
				error = glGetError();
				error = error;
			}

			delete[] subBuffer;
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void Texture::Set() //TESTING TESTING
{
	m_textureResource = new TextureResource;
	m_textureResource->id = 1;
	strcpy(m_textureResource->filePath, "C:/GameloftWorkshop/proiect_2015/OutsideResources/Textures/Croco.tga");
	m_textureResource->minFilter = GL_LINEAR;
	m_textureResource->magFilter = GL_LINEAR;
	m_textureResource->wrapS = GL_CLAMP_TO_EDGE;
	m_textureResource->wrapT = GL_CLAMP_TO_EDGE;
}

GLuint Texture::GetTextureId()
{
	return textureId;
}
