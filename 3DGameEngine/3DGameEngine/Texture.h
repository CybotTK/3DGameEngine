#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Utilities/utilities.h"

#include <string>

struct TextureResource {
	char filePath[256];
	int id;
	GLenum type;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapS;
	GLenum wrapT;

};

class Texture {
public:
	Texture();
	virtual ~Texture();

	void Load();

	void Set(); // TESTING TESTING

	GLuint GetTextureId();

	TextureResource* m_textureResource;

private:
	GLuint textureId; // Created by glGenTexture

	int width, height; // dimensiune texturi
	int bpp;		   // bits per pixel
	char* pixelArray;
};

#endif // !TEXTURE_H
