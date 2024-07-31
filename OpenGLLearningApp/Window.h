#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<stdio.h>
class Window
{
private:
	GLint height, width;
	GLFWwindow* mainWindow;
	int bufferWidth, bufferHeight;
	bool keys[1024];
	GLfloat xLast, yLast;
	GLfloat xChange, yChange;
	bool hasMouseMovedFirstTime;
	void SetEventCallBacks();

public:
	Window();
	~Window();
	int IntializeGLFWWindow();
	int GetBufferWidth();
	int GetBufferHeight();
	bool ShouldWindowClose();
	void WindowSwapBuffers();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	GLfloat GetXChange();
	GLfloat GetYChange();
	bool* GetKeys();
};

