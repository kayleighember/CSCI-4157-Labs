#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "VertexBuffer.h"
#include "IAnimation.h"

class IAnimation;

class GraphicsObject
{
protected:
	glm::mat4 referenceFrame;
	std::shared_ptr<VertexBuffer> buffer;
	GraphicsObject* parent;
	std::vector<std::shared_ptr<GraphicsObject>> children;
	std::shared_ptr<IAnimation> animation = nullptr;

public:
	GraphicsObject();
	virtual ~GraphicsObject();

	const glm::mat4 GetReferenceFrame() const;
	inline void SetReferenceFrame(glm::mat4 referenceFrame) { this->referenceFrame = referenceFrame; }
	void CreateVertexBuffer(unsigned int numberOfElementsPerVertex);
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	inline const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return buffer; }
	inline void SetAnimation(std::shared_ptr<IAnimation> animation) { this->animation = animation; }
	inline glm::mat4& GetLocalReferenceFrame() { return referenceFrame; }
	void StaticAllocateVertexBuffer();
	void AddChild(std::shared_ptr<GraphicsObject> child);
	inline const std::vector<std::shared_ptr<GraphicsObject>>& GetChildren() const { return children; }
	void SetPosition(const glm::vec3& position);
	void ResetOrientation();
	void RotateLocalX(float degrees);
	void RotateLocalY(float degrees);
	void RotateLocalZ(float degrees);
	void Update(double elapsedSeconds);
};

