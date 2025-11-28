#include "NewShader.h"

GLuint NewShader::GetUniformLocation(const std::string& uniformName)
{
	if (NameToLocation.find(uniformName) != NameToLocation.end())
	{
		return NameToLocation[uniformName];
	}

	GLuint location = glGetUniformLocation(shaderID, uniformName.c_str());
	NameToLocation[uniformName] = location;
	return location;
}

NewShader::NewShader()
{
}

void NewShader::CompileShaders(const std::string& filepath, std::initializer_list<std::string> uniformNames)
{
	BaseShader::CompileShaders(filepath);

	for (const std::string& s : uniformNames)
	{
		GetUniformLocation(s);
	}
}

void NewShader::setUniform1i(const std::string& uniformName, int var)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniform1i(location, var);
}

void NewShader::setUniform1f(const std::string& uniformName, GLfloat var)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniform1f(location, var);
}

void NewShader::setUniform3f(const std::string& uniformName, GLfloat var1, GLfloat var2, GLfloat var3)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniform3f(location, var1, var2, var3);
}

void NewShader::setUniform4f(const std::string& uniformName, GLfloat var1, GLfloat var2, GLfloat var3, GLfloat var4)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniform4f(location, var1, var2, var3, var4);
}

void NewShader::setUniformMatrix3fv(const std::string& uniformName, const glm::mat3& matrix)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}



void NewShader::setUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix)
{
	GLuint location = GetUniformLocation(uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}



NewShader::~NewShader()
{
}
