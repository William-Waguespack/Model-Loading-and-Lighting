#include "camera.h"

Camera::Camera(glm::mat4& projection) : projection(projection)
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = 4.0f;
	this->turnSpeed = 0.8f;

	update();
}

Camera::Camera(glm::vec3 postiion, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed, glm::mat4& projection) : projection(projection)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;

	update();
}

void Camera::keyControl(bool* keys, double deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		position += worldUp * velocity;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= worldUp * velocity;
	}

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, double deltaTime)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	// Clamp pitch
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update();
}

void Camera::SetMovementSpeed(GLfloat speed)
{
	movementSpeed = speed;
}

void Camera::SetProjection(glm::mat4& projection)
{
	this->projection = projection;
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}

void Camera::SetTurnSpeed(GLfloat turnSpeed)
{
	this->turnSpeed = turnSpeed;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

Camera::~Camera()
{
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
