#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;

	mouseFirstMoved = true;
	CursorMode = false;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;

	mouseFirstMoved = true;
	CursorMode = false;
}

GLFWwindow* Window::GetmainWindow()
{
	return mainWindow;
}

int Window::initialize()
{
	
	//initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed\n";
		glfwTerminate();
		return -1;
	}
	
	//Setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	//Windowed mode by default
	mainWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	
	//Fullscreen
	//mainWindow = glfwCreateWindow(width, height, "Hello World", glfwGetPrimaryMonitor(), NULL);
	//glfwSetWindowMonitor(mainWindow, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	//Windowed Fullscreen
	/*
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowMonitor(mainWindow, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	std::cout << "refresh rate is : " << mode->refreshRate << std::endl;*/

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	
	if (!mainWindow)
	{
		std::cout << "GLFW window created failed\n";
		glfwTerminate();
		return -1;
	}
	
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);
	
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(mainWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	else
	{
		std::cout << "Raw mouse motion not supported.\n";
	}

	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//Allows modern extension features
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialization failed\n";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}
	
	glEnable(GL_DEPTH_TEST);

	//Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);


	glfwSetWindowUserPointer(mainWindow, this);
	
	return 0;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));



	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if (CursorMode)
		{
			CursorMode = false;
			glfwSetInputMode(theWindow->mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			
		}
		else
		{
			CursorMode = true;
			
			glfwSetInputMode(theWindow->mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//glfwSetCursorPos(window, theWindow->width / 2.0, theWindow->height / 2.0);
			
		}
		
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	if(CursorMode)
	{
		theWindow->xChange = 0;
		theWindow->yChange = 0;
		return;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	//printf("x:%f, y:%f\n", theWindow->xChange, theWindow->yChange);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}
