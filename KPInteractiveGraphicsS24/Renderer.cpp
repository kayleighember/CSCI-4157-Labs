#include "Renderer.h"

#include <glad/glad.h>

Renderer::Renderer(std::shared_ptr<Shader> shader) {
	// set the shader
	this->_shader = shader;
	// generate the vaoid
	vaoId = 0;
	glGenVertexArrays(1, &vaoId);
}

Renderer::Renderer() {
	// idk do I need a default constructor??
}

void Renderer::StaticAllocVertexBuffers(const std::vector<std::shared_ptr<GraphicsObject>>& objects) {
	glBindVertexArray(vaoId);
	//auto& objects = scene->GetObjects();
	for (auto& object : objects) {
		object->StaticAllocateVertexBuffer();
	}
	glBindVertexArray(0);
}

void Renderer::RenderObject(const GraphicsObject& object)
{
	_shader->SendMat4Uniform("world", object.GetReferenceFrame());

	auto& buffer = object.GetVertexBuffer();
	buffer->Select();
	if (buffer->HasTexture()) {
		//glUniform1i(texUnit, buffer->GetTextureUnit());
		_shader->SendIntUniform("texUnit", buffer->GetTextureUnit());
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

void Renderer::RenderScene(std::shared_ptr<Scene>& scene, glm::mat4& view) {
	glUseProgram(_shader->GetShaderProgram());
	glBindVertexArray(vaoId);
	_shader->SendMat4Uniform("view", view);
	// Render the objects in the scene
	for (auto& object : scene->GetObjects()) {
		RenderObject(*object);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
	glBindVertexArray(0);
}
