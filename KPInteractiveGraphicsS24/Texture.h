#pragma once
#include "BaseObject.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture : public BaseObject
{
private:
	unsigned char* textureData;
	bool isLoadedFromFile;
	unsigned int textureId;
public:
	Texture();
	~Texture();
	unsigned char* GetTextureData();
	void SetTextureData(unsigned int count, unsigned char* data);
	bool IsLoadedFromFile();
	unsigned int GetTextureId();
	void Cleanup();
};

