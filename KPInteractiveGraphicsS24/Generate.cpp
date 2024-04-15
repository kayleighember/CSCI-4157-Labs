#include "Generate.h"

std::shared_ptr<VertexBuffer> Generate::Cuboid(
	float width, float height, float depth,
	glm::vec3 color, glm::vec2 tex) {
	std::shared_ptr<VertexBuffer> vertBuffer = std::make_shared<VertexBuffer>(8);
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float halfDepth = depth / 2;
	// Front Face
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// Right Face        
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// Back Face          
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// Left Face          
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// Top Face            
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// Bottom Face       
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, -halfHeight, -halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// 3 vertex per triangle, 2 triangles per face, 6 faces
	// 3 * 2 * 6 = 36 vertices
	vertBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertBuffer->AddVertexAttribute("color", 1, 3, 3);
	vertBuffer->AddVertexAttribute("texCoord", 2, 2, 6);
	return vertBuffer;
}

std::shared_ptr<VertexBuffer> Generate::XZPlane(
	float width, float depth, glm::vec3 color, glm::vec2 tex) {
	std::shared_ptr<VertexBuffer> vertBuffer = std::make_shared<VertexBuffer>(8);	// numElementsPerVertex
	float halfWidth = width / 2;
	float halfDepth = depth / 2;
	vertBuffer->AddVertexData(8, -halfWidth, 0.0f, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -halfWidth, 0.0f, halfDepth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, 0.0f, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -halfWidth, 0.0f, -halfDepth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, halfWidth, 0.0f, halfDepth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, halfWidth, 0.0f, -halfDepth, color[0], color[1], color[2], tex[0], tex[1]);
	// index, numberOfElements, offsetCount
	vertBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertBuffer->AddVertexAttribute("color", 1, 3, 3);
	vertBuffer->AddVertexAttribute("texCoord", 2, 2, 6);
	return vertBuffer;
}

