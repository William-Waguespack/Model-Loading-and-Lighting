#pragma once

#include <GL\glew.h>
#include <string>

enum class TextureType
{
	Diffuse,
	Specular,
	Normal,
	Height,
	Emissive
};

class Texture
{
public:
	
	Texture(const std::string& fileLoc, TextureType texType);
	Texture(std::string&& fileLoc, TextureType texType);

	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	Texture(Texture&& other) noexcept;

	Texture& operator=(Texture&& other) noexcept;

	~Texture();

	bool LoadTexture();
	void UseTexture(int unit);
	void ClearTexture();
	TextureType GetType();
	GLuint GetID();
	std::string GetFileLocation();
private:
	GLuint textureID;
	TextureType type;
	std::string fileLocation;
};

