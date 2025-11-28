#pragma once

#include <iostream>
#include <GL\glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:

	Window();
	Window(GLint windowWidth, GLint windowHeight);
	GLFWwindow* GetmainWindow();

	int initialize();

	~Window();

	GLfloat getXChange();

	GLfloat getYChange();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	inline static bool CursorMode = false;

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

	void createCallbacks();

public:
	inline GLfloat getBufferWidth()
	{
		return bufferWidth;
	}
	inline GLfloat getBufferHeight()
	{
		return bufferHeight;
	}
	inline bool GetShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	inline void SwapBuffers()
	{
		glfwSwapBuffers(mainWindow);
	}

	bool* GetKeys()
	{
		return keys;
	}
};



