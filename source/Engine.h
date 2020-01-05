#pragma once

struct GLFWwindow;

class Engine
{
public:
	Engine();
	~Engine();

	void Setup();
	void Loop();
	void Destroy();

private:
	bool SetupWindow();
	bool SetupRenderer();
	void PrintVersionInfo();

	GLFWwindow* myWindow;
};