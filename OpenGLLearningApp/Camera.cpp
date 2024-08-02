#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;
	update();
}

Camera::~Camera()
{
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = deltaTime * movementSpeed;
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
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	pitch += yChange;
	yaw += xChange;

	if (yaw > 89.0f)
	{
		yaw = 89.0f;
	}

	if (yaw < -89.0f)
	{
		yaw = -89.0f;
	}
	update();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return this->position;
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));	
}



