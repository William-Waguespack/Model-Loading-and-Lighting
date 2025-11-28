#pragma once
#include <iostream>
#include <string>
#include <GL\glew.h>



class Cubemap
{
	public:

		Cubemap(const std::string& rightPath, const std::string& leftPath, const std::string& topPath, const std::string& bottomPath, const std::string& frontPath, const std::string& backPath);
		void UseCubemap(int unit);
		~Cubemap();


	private:

		bool CreateCubemap();

		
		std::string facePaths[6];

		GLuint cubemapTexture;

		int width, height, bitDepth;
		
};

