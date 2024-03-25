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
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	// Cull back faces and use counter-clockwise winding of front faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	// Anti-aliasing
	glEnable(GL_MULTISAMPLE);

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

glm::mat4 GraphicsEnvironment::CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {
	glm::vec3 right = glm::cross(direction, up);
	right = glm::normalize(right);

	glm::vec3 vUp = glm::cross(right, direction);
	vUp = glm::normalize(vUp);

	glm::mat4 view(1.0f);
	view[0] = glm::vec4(right, 0.0f);
	view[1] = glm::vec4(up, 0.0f);
	view[2] = glm::vec4(direction, 0.0f);
	view[3] = glm::vec4(position, 1.0f);
	return glm::inverse(view);
}

void GraphicsEnvironment::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void GraphicsEnvironment::Run2D() {
	window = GetWindow();
	glViewport(0, 0, 1200, 800);
	//glfwMaximizeWindow(window);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspectRatio = width / (height * 1.0f);
	float left = -50.0f;
	float right = 50.0f;
	float bottom = -50.0f;
	float top = 50.0f;
	left *= aspectRatio;
	right *= aspectRatio;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

	float angle = 0, childAngle = 0;
	float cameraX = -10, cameraY = 0;
	
	ImGuiIO& io = ImGui::GetIO();

	std::shared_ptr<Renderer> renderer = GetRenderer("renderer");
	std::shared_ptr<Renderer> textureRenderer = GetRenderer("textureRenderer");

	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		glfwGetWindowSize(window, &width, &height);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		// Update the objects in the scene
		auto& objects = renderer->GetScene()->GetObjects();
		for (auto& object : objects) {
			object->ResetOrientation();
			object->RotateLocalZ(angle);
			for (auto& child : object->GetChildren()) {
				child->ResetOrientation();
				child->RotateLocalZ(childAngle);
			}
		}

		view = CreateViewMatrix(
			glm::vec3(cameraX, cameraY, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		renderer->SetProjection(projection);
		textureRenderer->SetProjection(projection);
		renderer->SetView(view);
		textureRenderer->SetView(view);
		Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		//ImGui::Text(shader->GetLog().c_str());
		//ImGui::Text(textureShader->GetLog().c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / io.Framerate, io.Framerate);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Angle", &angle, 0, 360);
		ImGui::SliderFloat("Child Angle", &childAngle, 0, 360);
		ImGui::SliderFloat("Camera X", &cameraX, left, right);
		ImGui::SliderFloat("Camera Y", &cameraY, bottom, top);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void GraphicsEnvironment::Run3D() {
	window = GetWindow();
	glViewport(0, 0, 1200, 800);
	//glfwMaximizeWindow(window);

	float cubeYAngle = 0;
	float cubeXAngle = 0;
	float cubeZAngle = 0;
	float left = -60.0f;
	float right = 60.0f;
	float bottom = -60.0f;
	float top = 60.0f;
	int width, height;
	float aspectRatio;
	float nearPlane = 1.0f;	// don't use 0 for near plane
	float farPlane = 60.0f;
	float fieldOfView = 60;	// vertical fov in radians
	glm::vec3 cameraPosition(15.0f, 15.0f, 20.0f);
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 referenceFrame(1.0f);
	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

	ImGuiIO& io = ImGui::GetIO();

	std::shared_ptr<Renderer> renderer = GetRenderer("renderer");
	std::vector<std::shared_ptr<GraphicsObject>> objects = renderer->GetScene()->GetObjects();

	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		glfwGetWindowSize(window, &width, &height);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		objects[0]->ResetOrientation();
		objects[0]->RotateLocalX(cubeXAngle);
		objects[0]->RotateLocalY(cubeYAngle);
		objects[0]->RotateLocalZ(cubeZAngle);

		view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

		if (width >= height) {
			aspectRatio = width / (height * 1.0f);
		}
		else {
			aspectRatio = height / (width * 1.0f);
		}
		projection = glm::perspective(
			glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
		
		renderer->SetProjection(projection);
		// why do you have 2 renderers here u dumdum
		renderer->SetView(view);
		Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		//ImGui::Text(message.c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / io.Framerate, io.Framerate);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("X Angle", &cubeXAngle, 0, 360);
		ImGui::SliderFloat("Y Angle", &cubeYAngle, 0, 360);
		ImGui::SliderFloat("Z Angle", &cubeZAngle, 0, 360);
		ImGui::SliderFloat("Camera X", &cameraPosition.x, left, right);
		ImGui::SliderFloat("Camera Y", &cameraPosition.y, bottom, top);
		ImGui::SliderFloat("Camera Z", &cameraPosition.z, -20, 50);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
