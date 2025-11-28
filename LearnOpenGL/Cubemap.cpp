#include "Cubemap.h"
#include "Image.h"


Cubemap::Cubemap(const std::string& rightPath, const std::string& leftPath, const std::string& topPath, const std::string& bottomPath, const std::string& frontPath, const std::string& backPath)
{
	facePaths[0] = rightPath;
	facePaths[1] = leftPath;
	facePaths[2] = topPath;
	facePaths[3] = bottomPath;
	facePaths[4] = frontPath;
	facePaths[5] = backPath;
	cubemapTexture = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	CreateCubemap();
}

void Cubemap::UseCubemap(int unit)
{
	
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

}

Cubemap::~Cubemap()
{
}

bool Cubemap::CreateCubemap()
{

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	
	for (int i = 0; i < 6; i++)
	{
		
		stbi_set_flip_vertically_on_load(false);
		unsigned char* textureData = stbi_load(facePaths[i].c_str(), &width, &height, &bitDepth, 0);
		if (!textureData)
		{
			printf("Failded to find: %s\n", facePaths[i].c_str());
			return false;
		}
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

		stbi_image_free(textureData);
	}
	return true;
}