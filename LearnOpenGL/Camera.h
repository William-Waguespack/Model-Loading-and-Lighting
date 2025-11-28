#pragma once

#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera
{
public:
	Camera(glm::mat4& projection);
	Camera(glm::vec3 postiion, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed, glm::mat4& projection);

	void keyControl(bool* keys, double deltaTime);

	void mouseControl(GLfloat xChange, GLfloat yChange, double deltaTime);

	void SetMovementSpeed(GLfloat speed);

	void SetProjection(glm::mat4& projection);

	glm::mat4 GetProjection();

	void SetTurnSpeed(GLfloat turnSpeed);

	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();

	glm::vec3 getCameraPosition();

	~Camera();

private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::mat4 projection;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

