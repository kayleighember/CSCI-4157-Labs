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
	std::shared_ptr<Scene> scene;
	glm::mat4 view;
	unsigned int vaoId;
public:
	Renderer(std::shared_ptr<Shader> shader);
	~Renderer() = default;
	inline const std::shared_ptr<Shader> GetShader() const { return _shader; }
	inline const std::shared_ptr<Scene> GetScene() const { return scene; }
	inline void SetScene(std::shared_ptr<Scene>& scene) { this->scene = scene; }
	inline void SetView(glm::mat4& view) { this->view = view; }
	void StaticAllocVertexBuffers(const std::vector<std::shared_ptr<GraphicsObject>>& objects);
	void RenderScene();
private:	
	void RenderObject(const GraphicsObject& object);
};

