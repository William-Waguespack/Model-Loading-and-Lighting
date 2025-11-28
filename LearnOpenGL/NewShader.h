#pragma once
#include "BaseShader.h"
#include <initializer_list>
#include <unordered_map>

class NewShader : public BaseShader
{
private:

	GLuint GetUniformLocation(const std::string& uniformName);

public:
	NewShader();

	using BaseShader::CompileShaders;
	void CompileShaders(const std::string& filepath, std::initializer_list<std::string> uniformNames);

	void setUniform1i(const std::string& uniformName, int var);

	void setUniform1f(const std::string& uniformName, GLfloat var);

	void setUniform3f(const std::string& uniformName, GLfloat var1, GLfloat var2, GLfloat var3);

	void setUniform4f(const std::string& uniformName, GLfloat var1, GLfloat var2, GLfloat var3, GLfloat var4);

	void setUniformMatrix3fv(const std::string& uniformName, const glm::mat3& matrix);

	void setUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix);

	

	~NewShader();

private:

	

	std::unordered_map<std::string, GLuint> NameToLocation;
};

