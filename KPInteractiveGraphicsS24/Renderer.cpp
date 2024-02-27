#include "Renderer.h"

#include <glad/glad.h>

Renderer::Renderer(std::shared_ptr<Shader> shader) {
	// set the shader
	this->shader = shader;
	// generate the vaoid
	vaoId = 0;
	glGenVertexArrays(1, &vaoId);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
}

void Renderer::StaticAllocVertexBuffers() {
	glBindVertexArray(vaoId);
	auto& objects = scene->GetObjects();
	for (auto& object : objects) {
		object->StaticAllocateVertexBuffer();
	}
	glBindVertexArray(0);
}

void Renderer::RenderObject(const GraphicsObject& object)
{
	shader->SendMat4Uniform("world", object.GetReferenceFrame());

	auto& buffer = object.GetVertexBuffer();
	buffer->Select();
	if (buffer->HasTexture()) {
		//glUniform1i(texUnit, buffer->GetTextureUnit());
		shader->SendIntUniform("texUnit", buffer->GetTextureUnit());
		// get texture from the buffer and select it for render
		buffer->SelectTexture();
	}
	buffer->SetUpAttributeInterpretration();
	glDrawArrays(buffer->GetPrimitiveType(), 0, buffer->GetNumberOfVertices());

	// Recursively render the children
	auto& children = object.GetChildren();
	for (auto& child : children) {
		RenderObject(*child);
	}
}

glm::mat4 Renderer::CreateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up)
{
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

void Renderer::RenderScene() {
	glUseProgram(shader->GetShaderProgram());
	glBindVertexArray(vaoId);
	shader->SendMat4Uniform("view", view);
	shader->SendMat4Uniform("proection", projection);
	// Render the objects in the scene
	//std::shared_ptr<Scene> scene = GetScene();
	for (auto& object : scene->GetObjects()) {
		RenderObject(*object);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
	glBindVertexArray(0);
}
