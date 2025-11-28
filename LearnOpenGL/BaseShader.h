#pragma once

#include <string>

#include <GL\glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

class BaseShader
{
public:
	BaseShader();

	void UseShader();
	void ClearShader();
	virtual void CompileShaders(const std::string& filepath);



	virtual ~BaseShader();

protected:

	GLuint shaderID;

	void ParseShader(const std::string& filepath, std::string& VertexSource, std::string& FramgentSource);
	void AddShader(GLuint Program, std::string& ShaderCode, GLenum shaderType);
};

