#include "Texture.h"
#include "GLError.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <sstream>

Texture::Texture(const std::string& aPath)
	: myID(0)
	, myWidth(0)
	, myHeight(0)
{
	const std::string source = ReadFile(aPath);
	if (source.empty())
		return;

	unsigned char* image = stbi_load(source.c_str(), &myWidth, &myHeight, &myComponents, STBI_default);
	if (!image)
	{
		printf("Failed to load texture: %s", source.c_str());
		return;
	}

	glGenTextures(1, &myID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, myID);

	GLenum format = myComponents == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, myWidth, myHeight, 0, format, GL_UNSIGNED_BYTE, image);

	CheckGLError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	CheckGLError();
}

Texture::~Texture()
{
}

std::string Texture::ReadFile(const std::string& aPath)
{
	std::string sourceResult;
	std::ifstream sourceStream(aPath, std::ios::in);

	if (!sourceStream.is_open())
	{
		printf("Failed to read file %s\n", aPath.c_str());
		return "";
	}

	std::stringstream sourceStringStream;
	sourceStringStream << sourceStream.rdbuf();
	sourceResult = sourceStringStream.str();
	sourceStream.close();

	return sourceResult;
}
