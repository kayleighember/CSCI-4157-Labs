#pragma once
#include "BaseObject.h"
#include "Shader.h"
#include "GraphicsObject.h"
#include "Scene.h"
#include <memory>

class Renderer : public BaseObject
{
private:
	std::shared_ptr<Shader> _shader;
	unsigned int vaoId;
public:
	Renderer(std::shared_ptr<Shader> shader);
	Renderer();
	~Renderer() = default;

	inline const std::shared_ptr<Shader> GetShader() const { return _shader; }
	void staticAllocVertexBuffers(const std::vector<std::shared_ptr<GraphicsObject>>& objects);
	void RenderScene(std::shared_ptr<Scene>& scene, glm::mat4& view);
private:	
	void RenderObject(const GraphicsObject& object);
};

