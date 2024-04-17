#include "GraphicsEnvironment.h"

GraphicsEnvironment* GraphicsEnvironment::self;

GraphicsEnvironment::GraphicsEnvironment() {
	window = nullptr;
	objManager = std::make_shared<ObjectManager>();
	camera = std::make_shared<Camera>();
	self = this;
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
	glfwSetCursorPosCallback(window, OnMouseMove);
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

glm::mat4 GraphicsEnvironment::CreateViewMatrix(glm::mat4& cameraFrame) {
	glm::mat4 view(1.0f);
	glm::vec3 cameraForward;
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraPosition = cameraFrame[3];
	cameraForward = cameraFrame[2];
	cameraForward = -cameraForward;
	cameraTarget = cameraPosition + cameraForward;
	cameraUp = cameraFrame[1];
	view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

	return view;
}

void GraphicsEnvironment::AddObject(const std::string& name, std::shared_ptr<GraphicsObject> object) {
	objManager->SetObject(name, object);
}

void GraphicsEnvironment::ProcessInput(double elapsedSeconds) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	/*if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		axis = glm::vec3(0.0f, 1.0f, 0.0f);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
		return;
	}*/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {	// forward
		camera->MoveForward(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {	// left
		camera->MoveLeft(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {	// backward
		camera->MoveBackward(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {	// right
		camera->MoveRight(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // up
		camera->MoveUp(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { // down
		camera->MoveDown(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {	// everybody look left
		camera->LookLeft(elapsedSeconds);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {	// everybody look right
		camera->LookRight(elapsedSeconds);
		return;
	}
	
}

void GraphicsEnvironment::OnMouseMove(GLFWwindow* window, double mouseX, double mouseY) {
	self->mouse.x = mouseX;
	self->mouse.y = mouseY;

	float xPercent = static_cast<float>(self->mouse.x / self->mouse.windowWidth);
	float yPercent = static_cast<float>(self->mouse.y / self->mouse.windowHeight);

	self->mouse.spherical.theta = 90.0f - (xPercent * 180); // left/right
	self->mouse.spherical.phi = 180.0f - (yPercent * 180); // up/down
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
		// ProcessInput(window);
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

	bool resetCameraPosition = false;
	bool lookWithMouse = false;
	float cubeYAngle = 0;
	float cubeXAngle = 0;
	float cubeZAngle = 0;
	float left = -20.0f;
	float right = 20.0f;
	float bottom = -20.0f;
	float top = 20.0f;
	int width, height;

	float aspectRatio;
	float nearPlane = 1.0f;	// don't use 0 for near plane
	float farPlane = 100.0f;
	float fieldOfView = 60;	// vertical fov in radians

	//glm::vec3 cameraPosition(15.0f, 15.0f, 20.0f);
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 referenceFrame(1.0f);
	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };	
	
	//glm::mat4 cameraFrame(1.0f);
	//cameraFrame[3] = glm::vec4(0.0f, 3.0f, 20.0f, 1.0f);
	//glm::vec3 cameraForward;
	//glm::vec3 axis(0.0f, 1.0f, 0.0f);

	float speed = 90.0f;
	double elapsedSeconds;
	//float deltaAngle;
	Timer timer;

	ImGuiIO& io = ImGui::GetIO();

	std::shared_ptr<Renderer> renderer = GetRenderer("renderer");
	std::vector<std::shared_ptr<GraphicsObject>> objects = renderer->GetScene()->GetObjects();

	std::shared_ptr<RotateAnimation> rotateAnimation = std::make_shared<RotateAnimation>();
	rotateAnimation->SetObject(objManager->GetObject("crate"));
	objManager->GetObject("crate")->SetAnimation(rotateAnimation);
	objManager->GetObject("crate")->SetPosition({-10.0f, 2.5f, 0.0f});

	camera->SetPosition({ 0.0f, 5.0f, 20.0f });

	while (!glfwWindowShouldClose(window)) {
		elapsedSeconds = timer.GetElapsedTimeInSeconds();
		ProcessInput(elapsedSeconds);
		glfwGetWindowSize(window, &width, &height);
		mouse.windowWidth = width;
		mouse.windowHeight = height;
		

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//deltaAngle = static_cast<float>(speed * elapsedSeconds);
		//referenceFrame = glm::rotate(referenceFrame, glm::radians(deltaAngle), axis);
		
		if (lookWithMouse) {
			camera->SetLookFrame(mouse.spherical.ToMat4());
		}	
		if (resetCameraPosition) {
			lookWithMouse = false;
			camera->SetLookFrame(glm::mat4(1.0f));
			resetCameraPosition = false;
		}
		view = camera->LookForward();		

		if (width >= height) {
			aspectRatio = width / (height * 1.0f);
		}
		else {
			aspectRatio = height / (width * 1.0f);
		}
		projection = glm::perspective(
			glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);		
		renderer->SetProjection(projection);		
		renderer->SetView(view);
		objManager->Update(elapsedSeconds);
		Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		//ImGui::Text(message.c_str());
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("Elapsed seconds: %.3f", elapsedSeconds);
		ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
		ImGui::SliderFloat("Speed", &speed, 0, 360);
		ImGui::Checkbox("Use mouse to look", &lookWithMouse);
		ImGui::Checkbox("Reset camera position", &resetCameraPosition);
		//ImGui::SliderFloat("X Angle", &cubeXAngle, 0, 360);
		//ImGui::SliderFloat("Y Angle", &cubeYAngle, 0, 360);
		//ImGui::SliderFloat("Z Angle", &cubeZAngle, 0, 360);
		//ImGui::SliderFloat("Speed", &camera->GetSpeed(), 0, 360);
		// label, currentValue, min, max
		ImGui::SliderFloat("Camera X", &camera->GetReferenceFrame()[3].x, left, right);
		ImGui::SliderFloat("Camera Y", &camera->GetReferenceFrame()[3].y, bottom, top);
		ImGui::SliderFloat("Camera Z", &camera->GetReferenceFrame()[3].z, 20, 50);
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
