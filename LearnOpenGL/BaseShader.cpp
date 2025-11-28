
#include "BaseShader.h"
#include <iostream>
#include <fstream>
#include <stdio.h>


BaseShader::BaseShader()
{
	shaderID = 0;
}

void BaseShader::UseShader()
{
	glUseProgram(shaderID);
}

void BaseShader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
}

void BaseShader::CompileShaders(const std::string& filepath)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		std::cout << "Error creating shader\n";
		return;
	}

	std::string VertexShader;
	std::string FragmentShader;
	ParseShader(filepath, VertexShader, FragmentShader);
	AddShader(shaderID, VertexShader, GL_VERTEX_SHADER);
	AddShader(shaderID, FragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

BaseShader::~BaseShader()
{
	ClearShader();
}

void BaseShader::ParseShader(const std::string& filepath, std::string& VertexSource, std::string& FramgentSource)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			if (type == ShaderType::VERTEX)
			{
				VertexSource += line + "\n";
			}
			else if (type == ShaderType::FRAGMENT)
			{
				FramgentSource += line + "\n";
			}
		}
	}
}

void BaseShader::AddShader(GLuint Program, std::string& ShaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode = ShaderCode.c_str();
	GLint codeLength = strlen(theCode);


	glShaderSource(theShader, 1, &theCode, &codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(Program, theShader);
}
