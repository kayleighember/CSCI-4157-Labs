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
	inline const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const {
		return buffer;
	}
	void SetPosition(const glm::vec3& position);
	void ResetOrientation();
	void RotateLocalZ(float degrees);
};

