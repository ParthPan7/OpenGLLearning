#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
	~Camera();
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl (GLfloat xChange, GLfloat yChange);
	glm::mat4 GetViewMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat turnSpeed;
	void update();
};

