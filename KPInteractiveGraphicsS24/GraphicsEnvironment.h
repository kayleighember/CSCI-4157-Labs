#pragma once
#include "BaseObject.h"
#include "Renderer.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "RotateAnimation.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <unordered_map>

class GraphicsEnvironment : public BaseObject
{
private:
	GLFWwindow* window;
	std::unordered_map<std::string, std::shared_ptr<Renderer>> rendererMap;
	// create this, don't just define!
	std::shared_ptr<ObjectManager> objManager;
public:
	GraphicsEnvironment();
	~GraphicsEnvironment();
	inline GLFWwindow* GetWindow() { return window; }
	void Init(unsigned int majorVersion, unsigned int minorVersion);
	bool SetWindow(unsigned int width, unsigned int height, const std::string& title);
	bool InitGlad();
	void SetUpGraphics();
	static void OnWindowSizeChanged(GLFWwindow* window, int width, int height);
	void CreateRenderer(const std::string& name, std::shared_ptr<Shader> shader);
	std::shared_ptr<Renderer> GetRenderer(const std::string& name);
	void StaticAllocate();
	void Render();
	static glm::mat4 CreateViewMatrix(glm::mat4& cameraFrame);
	static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
	void ProcessInput(GLFWwindow* window);
	void ProcessInput(GLFWwindow* window, double elapsedSeconds, glm::vec3& axis, glm::mat4& cameraFrame);
	void Run2D();
	void Run3D();
	void AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object);
};

