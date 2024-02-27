#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Renderer.h"
#include "TextFile.h"
#include "GraphicsEnvironment.h"

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

static void SetUpTexturedScene(
	std::shared_ptr<Shader>& textureShader, std::shared_ptr<Scene>& textureScene) {
	TextFile file;
	file.TextToStringStream("texture.vert.glsl");
	std::string vertexSource = file.data;
	file.TextToStringStream("texture.frag.glsl");
	std::string fragmentSource = file.data;

	textureShader = std::make_shared<Shader>(vertexSource, fragmentSource);
	textureShader->AddUniform("projection");
	textureShader->AddUniform("world");
	textureShader->AddUniform("view");
	textureShader->AddUniform("texUnit");
	unsigned int shaderProgram = textureShader->GetShaderProgram();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->SetWidthHeight(4,4);
	unsigned char* textureData = new unsigned char[] {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
		255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	texture->SetTextureData(64, textureData);
	
	textureScene = std::make_shared<Scene>();
	std::shared_ptr<GraphicsObject> graphicsObj = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(8);
	buffer->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	buffer->AddVertexData(8, -20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	buffer->AddVertexData(8, -20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f);
	buffer->AddVertexData(8, 20.0f, -20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f);
	buffer->AddVertexData(8, 20.0f, 20.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 3.0f);
	buffer->AddVertexAttribute("position", 0, 3, 0);
	buffer->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer->AddVertexAttribute("texCoord", 2, 2, 6);
	texture->SetWrapS(GL_CLAMP_TO_EDGE);
	texture->SetWrapT(GL_CLAMP_TO_EDGE);
	texture->SetMagFilter(GL_LINEAR);
	buffer->SetTexture(texture);
	graphicsObj->SetVertexBuffer(buffer);
	graphicsObj->SetPosition(glm::vec3(-30, -10, 0));
	textureScene->AddObject(graphicsObj);

	// add a new textured object to your scene
	std::shared_ptr<GraphicsObject> graphicsObj2 = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer2 = std::make_shared<VertexBuffer>(8);
	buffer2->AddVertexData(8, -10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	buffer2->AddVertexData(8, -10.0f, -10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	buffer2->AddVertexData(8, 10.0f, -10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(8, -10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	buffer2->AddVertexData(8, 10.0f, -10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(8, 10.0f, 10.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	buffer2->AddVertexAttribute("position", 0, 3, 0);
	buffer2->AddVertexAttribute("vertexColor", 1, 3, 3);
	buffer2->AddVertexAttribute("texCoord", 2, 2, 6);

	std::shared_ptr<Texture> texture2 = std::make_shared<Texture>();
	texture2->LoadTextureDataFromFile("pattern_0011.png");

	buffer2->SetTexture(texture2);
	graphicsObj2->SetVertexBuffer(buffer2);
	graphicsObj2->SetPosition(glm::vec3(26, 28, 0));
	textureScene->AddObject(graphicsObj2);
}

void SetUpBasicScene(std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene) {
	TextFile file;
	file.TextToStringStream("basic.vert.glsl");
	std::string vertexSource = file.data;
	file.TextToStringStream("basic.frag.glsl");
	std::string fragmentSource = file.data;

	shader = std::make_shared<Shader>(vertexSource, fragmentSource);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	unsigned int shaderProgram = shader->GetShaderProgram();

	scene = std::make_shared<Scene>();
	std::shared_ptr<GraphicsObject> square = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(6);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	buffer->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexData(6, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	buffer->AddVertexAttribute("position", 0, 3);
	buffer->AddVertexAttribute("color", 1, 3, 3);
	square->SetVertexBuffer(buffer);
	scene->AddObject(square);

	std::shared_ptr<GraphicsObject> triangle = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer2 = std::make_shared<VertexBuffer>(6);
	buffer2->AddVertexData(6, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, -5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexData(6, 5.0f, -5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer2->AddVertexAttribute("position", 0, 3);
	buffer2->AddVertexAttribute("color", 1, 3, 3);
	triangle->SetVertexBuffer(buffer2);
	triangle->SetPosition(glm::vec3(30.0f, 0.0f, 0.0f));
	scene->AddObject(triangle);

	std::shared_ptr<GraphicsObject> line = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> buffer3 = std::make_shared<VertexBuffer>(6);
	buffer3->SetPrimitiveType(GL_LINES);
	buffer3->AddVertexData(6, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexData(6, 0.0f, -2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	buffer3->AddVertexAttribute("position", 0, 3);
	buffer3->AddVertexAttribute("color", 1, 3, 3);
	line->SetVertexBuffer(buffer3);
	line->SetPosition(glm::vec3(5.0f, -10.0f, 0.0f));
	triangle->AddChild(line);
}

//static glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up)
//{
//	glm::vec3 right = glm::cross(direction, up);
//	right = glm::normalize(right);
//
//	glm::vec3 vUp = glm::cross(right, direction);
//	vUp = glm::normalize(vUp);
//
//	glm::mat4 view(1.0f);
//	view[0] = glm::vec4(right, 0.0f);
//	view[1] = glm::vec4(up, 0.0f);
//	view[2] = glm::vec4(direction, 0.0f);
//	view[3] = glm::vec4(position, 1.0f);
//	return glm::inverse(view);
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	std::shared_ptr<GraphicsEnvironment> environment = std::make_shared<GraphicsEnvironment>();
	environment->Init(4,3);
	if (!environment->SetWindow(1200, 800, "ETSU Computing Interactive Graphics")) { return -1;	}
	if (!environment->InitGlad()) { return -1; }
	environment->SetUpGraphics();
	GLFWwindow* window = environment->GetWindow();
	
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

#pragma region basicShape
	std::shared_ptr<Shader> basicShader;
	std::shared_ptr<Scene> basicScene;
	SetUpBasicScene(basicShader, basicScene);
	environment->CreateRenderer("renderer", basicShader);
	environment->GetRenderer("renderer")->SetScene(basicScene);
	auto& objects = basicScene->GetObjects();
#pragma endregion

#pragma region texturedShape
	std::shared_ptr<Shader> textureShader;
	std::shared_ptr<Scene> textureScene;
	// Call the function to set up the textured scene by passing the declared shader and scene
	SetUpTexturedScene(textureShader, textureScene);
	// Create textureRenderer using the new shader
	environment->CreateRenderer("textureRenderer", textureShader);
	environment->GetRenderer("textureRenderer")->SetScene(textureScene);
	auto& textureObjects = textureScene->GetObjects();
#pragma endregion

	glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

	//basicShader->SendMat4Uniform("projection", projection);
	//glUseProgram(textureShader->GetShaderProgram());
	//textureShader->SendMat4Uniform("projection", projection);

	float angle = 0, childAngle = 0;
	float cameraX = -10, cameraY = 0;
	glm::mat4 view;

	ImGuiIO& io = ImGui::GetIO();

	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);
		glfwGetWindowSize(window, &width, &height);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		// Update the objects in the scene
		for (auto& object : objects) {
			object->ResetOrientation();
			object->RotateLocalZ(angle);
			for (auto& child : object->GetChildren()) {
				child->ResetOrientation();
				child->RotateLocalZ(childAngle);
			}
		}

		view = environment->GetRenderer("renderer")->CreateViewMatrix(
			glm::vec3(cameraX, cameraY, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		environment->StaticAllocate();
		environment->GetRenderer("renderer")->SetProjection(projection);
		environment->GetRenderer("textureRenderer")->SetProjection(projection);
		environment->GetRenderer("renderer")->SetView(view);
		environment->GetRenderer("textureRenderer")->SetView(view);
		environment->Render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Computing Interactive Graphics");
		//ImGui::Text(shader->GetLog().c_str());
		ImGui::Text(textureShader->GetLog().c_str());
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
	return 0;
}

