#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "VertexBuffer.h"

class GraphicsObject
{
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> buffer;

public:
	GraphicsObject();
	virtual ~GraphicsObject();

	inline const glm::mat4 GetReferenceFrame() const { return referenceFrame; }
	void CreateVertexBuffer(unsigned int numberOfElementsPerVertex);
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	void ResetReferenceFrame() { referenceFrame = glm::mat4(1.0f); }
	void RotateLocalZ(float degrees);
};

