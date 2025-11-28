
#include "Texture.h"
#include "Image.h"
#include <iostream>



Texture::Texture(const std::string& fileLoc, TextureType texType)
{
	std::cout << "Created Texture by value " << fileLoc << " " << "\n";
	textureID = 0;
	type = texType;
	fileLocation = fileLoc;
}

Texture::Texture(std::string&& fileLoc, TextureType texType) : fileLocation(std::move(fileLoc))
{
	std::cout << "Created Texture by R value Ref" << fileLocation << " " << "\n";
	textureID = 0;
	type = texType;
}

Texture::Texture(Texture&& other) noexcept
{
	std::cout << "Created Texture by Move Constructor\n";
	this->type = other.type;
	this->fileLocation = other.fileLocation;
	this->textureID = other.textureID;

	other.textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	std::cout << "Altered Texture by Move assignment\n";
	this->type = other.type;
	this->fileLocation = other.fileLocation;
	this->textureID = other.textureID;

	other.textureID = 0;
	
	return *this;
}

Texture::~Texture()
{
	std::cout << "deleting Texture " << fileLocation.c_str() << "\n";
	ClearTexture();
}

bool Texture::LoadTexture()
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, bitDepth;
	unsigned char* textureData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
	if (!textureData)
	{
		printf("Failded to find: %s\n", fileLocation.c_str());
		return false;
	}

	GLenum format = GL_RGB;

	if (bitDepth == 1)
	{
		format = GL_RED;
	}
	else if (bitDepth == 3)
	{
		format = GL_RGB;
	}
	else if (bitDepth == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(textureData);
	return true;
}

void Texture::UseTexture(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glBindTextureUnit(unit, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
}

TextureType Texture::GetType()
{
	return type;
}

GLuint Texture::GetID()
{
	return textureID;
}

std::string Texture::GetFileLocation()
{
	return fileLocation;
}
