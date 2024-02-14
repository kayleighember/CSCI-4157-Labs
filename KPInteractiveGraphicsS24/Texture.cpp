#include "Texture.h"

Texture::Texture() {
	textureData = nullptr;
	isLoadedFromFile = false;
	glGenTextures(1, &textureId);
}

Texture::~Texture() {
	Cleanup();
}

unsigned char* Texture::GetTextureData() {
	return textureData;
}

void Texture::SetTextureData(unsigned int count, unsigned char* data) {
	Cleanup();
	textureData = new unsigned char[count];
	memcpy(data, textureData, count);
	isLoadedFromFile = false;
}

unsigned int Texture::GetTextureId() {
	return textureId;
}

bool Texture::IsLoadedFromFile() {
	return isLoadedFromFile;
}

void Texture::Cleanup() {
	if (textureData == nullptr) {
		return;
	}
	if (!isLoadedFromFile) {
		delete[] textureData;
		textureData = nullptr;
	}
}
