#include "GraphicsEnvironment.h"

GraphicsEnvironment::GraphicsEnvironment() {
	window = nullptr;
}

GraphicsEnvironment::~GraphicsEnvironment() {
	glfwTerminate();
}

void GraphicsEnvironment::Init(unsigned int majorVersion, unsigned int minorVersion) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool GraphicsEnvironment::SetWindow(unsigned int width, unsigned int height, const std::string& title) {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {		
		_log << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

bool GraphicsEnvironment::InitGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		_log << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

void GraphicsEnvironment::SetUpGraphics() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

void GraphicsEnvironment::OnWindowSizeChanged(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GraphicsEnvironment::CreateRenderer(const std::string& name, std::shared_ptr<Shader> shader) {
	// creates a Renderer using the shader and adds it to the map
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(shader);
	rendererMap[name] = renderer;
}

std::shared_ptr<Renderer> GraphicsEnvironment::GetRenderer(const std::string& name) {
	return rendererMap[name];
}

void GraphicsEnvironment::StaticAllocate() {
	for (const std::pair<const std::string, std::shared_ptr<Renderer>>& pair : rendererMap) {
		pair.second->StaticAllocVertexBuffers();
	}
}

void GraphicsEnvironment::Render() {
	for (const std::pair<const std::string, std::shared_ptr<Renderer>>& pair : rendererMap) {
		pair.second->RenderScene();
	}
}
