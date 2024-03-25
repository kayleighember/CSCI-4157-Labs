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
	unsigned int type, width, height, internalFormat, sourceFormat, wrapS, wrapT, magFilter, minFilter;
	int numberOfChannels;
public:
	Texture();
	~Texture();
	unsigned char* GetTextureData();
	void SetTextureData(unsigned int count, unsigned char* data);
	bool IsLoadedFromFile();
	unsigned int GetTextureId();
	void SetWidthHeight(unsigned int width, unsigned int height);
	inline void SetWrapS(unsigned int wrapS) { this->wrapS = wrapS; }
	inline void SetWrapT(unsigned int wrapT) { this->wrapT = wrapT; }
	inline void SetMagFilter(unsigned int magFilter) { this->magFilter = magFilter; }
	inline void SetMinFilter(unsigned int minFilter) { this->minFilter = minFilter; }
	inline int GetNumberOfChannels() { return numberOfChannels; }
	void LoadTextureDataFromFile(const std::string& filepath);
	void SelectToChange();
	void Deselect();
	void SelectToRender(int textureUnit = 0);
	void Allocate();
	void Cleanup();
};

