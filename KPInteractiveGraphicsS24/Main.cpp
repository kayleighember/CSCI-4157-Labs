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
#include "Generate.h"

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
	texture->SetWidthHeight(4, 4);
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
	graphicsObj->SetPosition(glm::vec3(-30, -15, 0));
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

static void SetUp3DScene1(std::shared_ptr<Shader>& shader, std::shared_ptr<Scene>& scene, GraphicsEnvironment& environment) {
	TextFile file;
	file.TextToStringStream("3D.vert.glsl");
	std::string vertexSource = file.data;
	file.TextToStringStream("3D.frag.glsl");
	std::string fragmentSource = file.data;
	
	shader = std::make_shared<Shader>(vertexSource, fragmentSource);
	shader->AddUniform("projection");
	shader->AddUniform("world");
	shader->AddUniform("view");
	unsigned int shaderProgram = shader->GetShaderProgram();

	std::shared_ptr<Texture> cubeTexture = std::make_shared<Texture>();
	std::shared_ptr<Texture> crateTexture = std::make_shared<Texture>();
	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>();
	cubeTexture->SetWidthHeight(4, 4);

	// Create the texture data
	unsigned char* textureData = new unsigned char[] {
		255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255, 255,
			0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
			0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 0, 255,
			255, 255, 255, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 255, 255, 255
	};
	cubeTexture->SetTextureData(64, textureData);
	crateTexture->LoadTextureDataFromFile("Wood_Crate_001.jpg");
	floorTexture->LoadTextureDataFromFile("floor_tiles.jpg");

	scene = std::make_shared<Scene>();
	// Cube
	std::shared_ptr<GraphicsObject> cube = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> cubeBuffer = Generate::Cuboid(10.0f, 5.0f, 5.0f);	
	cubeTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	cubeTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	cubeTexture->SetMagFilter(GL_NEAREST);
	cubeBuffer->SetTexture(cubeTexture);
	cube->SetVertexBuffer(cubeBuffer);
	cube->SetPosition(glm::vec3(0, 0, 0));
	scene->AddObject(cube);
	// Crate
	std::shared_ptr<GraphicsObject> crate = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> crateBuffer = Generate::Cuboid(5.0f, 5.0f, 5.0f);
	crateTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	crateTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	crateTexture->SetMagFilter(GL_LINEAR);
	crateBuffer->SetTexture(crateTexture);
	crate->SetVertexBuffer(crateBuffer);
	crate->SetPosition(glm::vec3(-20, 0, 0));
	scene->AddObject(crate);
	// Floor
	std::shared_ptr<GraphicsObject> floor = std::make_shared<GraphicsObject>();
	std::shared_ptr<VertexBuffer> floorBuffer = Generate::XZPlane(30.0f, 20.0f);
	floorTexture->SetWrapS(GL_REPEAT);
	floorTexture->SetWrapT(GL_REPEAT);
	floorTexture->SetMagFilter(GL_LINEAR);
	floorBuffer->SetTexture(floorTexture);
	floor->SetVertexBuffer(floorBuffer);
	floor->SetPosition(glm::vec3(0, -5, 0));
	scene->AddObject(floor);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GraphicsEnvironment environment;
	environment.Init(4,3);
	if (!environment.SetWindow(1200, 800, "ETSU Computing Interactive Graphics")) { return -1;	}
	if (!environment.InitGlad()) { return -1; }
	environment.SetUpGraphics();

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Scene> scene;
	SetUp3DScene1(shader, scene, environment);

	environment.CreateRenderer("renderer", shader);
	environment.GetRenderer("renderer")->SetScene(scene);
	environment.StaticAllocate();

	environment.Run3D();
	return 0;
}

