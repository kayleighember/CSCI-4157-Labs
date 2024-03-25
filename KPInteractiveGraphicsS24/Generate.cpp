#include "Generate.h"

std::shared_ptr<VertexBuffer> Generate::Cuboid(
	float width, float height, float depth,
	glm::vec3 color, glm::vec2 tex) {
	std::shared_ptr<VertexBuffer> vertBuffer = std::make_shared<VertexBuffer>(8);
	// Front Face
	vertBuffer->AddVertexData(8, -width, height, depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, -height, depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, height, depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, height, depth, color[0], color[1], color[2], tex[0], tex[1]);
	// Right Face        
	vertBuffer->AddVertexData(8, width, height, depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, -height, depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, width, -height, -depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, height, depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, -height, -depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, height, -depth, color[0], color[1], color[2], tex[0], tex[1]);
	// Back Face          
	vertBuffer->AddVertexData(8, width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, -height, -depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -width, -height, -depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, -height, -depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, height, -depth, color[0], color[1], color[2], tex[0], tex[1]);
	// Left Face          
	vertBuffer->AddVertexData(8, -width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, -height, -depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, height, depth, color[0], color[1], color[2], tex[0], tex[1]);
	// Top Face            
	vertBuffer->AddVertexData(8, -width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, height, depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, width, height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, height, -depth, color[0], color[1], color[2], tex[0], tex[1]);
	// Bottom Face       
	vertBuffer->AddVertexData(8, width, -height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, -height, depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, -width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, -height, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, -height, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, -height, -depth, color[0], color[1], color[2], tex[0], tex[1]);
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
	vertBuffer->AddVertexData(8, -width, 0.0f, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, -width, 0.0f, depth, color[0], color[1], color[2], 0.0f, 0.0f);
	vertBuffer->AddVertexData(8, width, 0.0f, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, -width, 0.0f, -depth, color[0], color[1], color[2], 0.0f, tex[1]);
	vertBuffer->AddVertexData(8, width, 0.0f, depth, color[0], color[1], color[2], tex[0], 0.0f);
	vertBuffer->AddVertexData(8, width, 0.0f, -depth, color[0], color[1], color[2], tex[0], tex[1]);
	// index, numberOfElements, offsetCount
	vertBuffer->AddVertexAttribute("position", 0, 3, 0);
	vertBuffer->AddVertexAttribute("color", 1, 3, 3);
	vertBuffer->AddVertexAttribute("texCoord", 2, 2, 6);
	return vertBuffer;
}

