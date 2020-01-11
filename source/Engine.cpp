#include "Engine.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <exception>

Engine::Engine()
	: myWindow(nullptr)
{
}

Engine::~Engine()
{
}

static void ErrorCallback(int anError, const char* aDescription)
{
	fprintf(stderr, "Error: %s\n", aDescription);
}

static void FrameBufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
}

static void KeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int anAction, int aMode)
{
	if (aKey == GLFW_KEY_ESCAPE && anAction == GLFW_PRESS)
		glfwSetWindowShouldClose(aWindow, GL_TRUE);
}

void Engine::Setup()
{
	if (!SetupWindow())
		throw std::exception();

	if (!SetupRenderer())
		throw std::exception();

	PrintVersionInfo();
}

void Engine::Loop()
{
	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	double deltaTime;

	while (!glfwWindowShouldClose(myWindow))
	{
		int width, height;
		float ratio;
		glfwGetFramebufferSize(myWindow, &width, &height);
		ratio = width / (float)height;

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glViewport(0, 0, width, height);
		glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//printf("dt %f\n", deltaTime);

		glfwSwapBuffers(myWindow);
		glfwPollEvents();
	}
}

void Engine::Render()
{

}

void Engine::Destroy()
{
	glfwDestroyWindow(myWindow);
	glfwTerminate();
}

bool Engine::SetupWindow()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	myWindow = glfwCreateWindow(1240, 720, "Hello OpenGL 4.6", nullptr, nullptr);

	if (myWindow == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(myWindow);

	glfwSetErrorCallback(ErrorCallback);

	glfwSetFramebufferSizeCallback(myWindow, FrameBufferSizeCallback);

	glfwSetKeyCallback(myWindow, KeyCallback);

	glfwSetInputMode(myWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSwapInterval(1);

	return true;
}

bool Engine::SetupRenderer()
{
	return gladLoadGL();
}

void Engine::PrintVersionInfo()
{
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("GLFW %d.%d.%d\n", major, minor, revision);
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}