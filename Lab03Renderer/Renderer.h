#pragma once
#include "BaseObject.h"
#include "Shader.h"

class Renderer : public BaseObject
{
private:
	std::shared_ptr<Shader> _shader;
public:
	//Renderer(std::shared_ptr<Shader> shader) : _shader(shader) {};
	Renderer();
	~Renderer() = default;
};

