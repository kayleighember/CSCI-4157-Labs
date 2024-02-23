#pragma once
#include "BaseObject.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

class GraphicsEnvironment : public BaseObject
{
private:
	GLFWwindow* window;
public:
	GraphicsEnvironment();
	~GraphicsEnvironment();
	inline GLFWwindow* GetWindow() { return window; }
	void Init(unsigned int majorVersion, unsigned int minorVersion);
	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad();
	void SetUpGraphics();
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
};

