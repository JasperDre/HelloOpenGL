#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ErrorCallback(int anError, const char* aDescription)
{
	fprintf(stderr, "Error: %s\n", aDescription);
}

static void FrameBufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
}

void KeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int anAction, int aMode)
{
	if (aKey == GLFW_KEY_ESCAPE && anAction == GLFW_PRESS)
		glfwSetWindowShouldClose(aWindow, GL_TRUE);
}

int main(int argc, char** argv)
{
	// Init GLFW
	GLFWwindow* window;

	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1240, 720, "Hello OpenGL", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);

	glfwSetFramebufferSizeCallback(myWindow, FrameBufferSizeCallback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init GLAD
	if (!gladLoadGL())
	{
		printf("Something went wrong!\n");
		exit(-1);
	}

	glfwSwapInterval(1);

	// Print version numbers
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("GLFW %d.%d.%d\n", major, minor, revision);
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	double deltaTime;

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		float ratio;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glViewport(0, 0, width, height);
		glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		printf("dt %f\n", deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroy
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
