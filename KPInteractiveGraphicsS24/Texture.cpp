#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	textureData = nullptr;
	isLoadedFromFile = false;
	glGenTextures(1, &textureId);
	type = GL_TEXTURE_2D;
	width = 0;
	height = 0;
	internalFormat = GL_RGBA8;
	sourceFormat = GL_RGBA;
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	magFilter = GL_NEAREST;
	minFilter = GL_NEAREST;
	numberOfChannels = 4;
}

Texture::~Texture() {
	Cleanup();
	glDeleteTextures(1, &textureId);
}

unsigned char* Texture::GetTextureData() {
	return textureData;
}

void Texture::SetTextureData(unsigned int count, unsigned char* data) {
	Cleanup();
	textureData = new unsigned char[count];
	memcpy(textureData, data, count);
	isLoadedFromFile = false;
}

unsigned int Texture::GetTextureId() {
	return textureId;
}

bool Texture::IsLoadedFromFile() {
	return isLoadedFromFile;
}

void Texture::SetWidthHeight(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

void Texture::LoadTextureDataFromFile(const std::string& filepath) {
	Cleanup();
	int width, height;
	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load(filepath.c_str(), &width, &height, &numberOfChannels, 0);
	this->width = width;
	this->height = height;
	if (numberOfChannels == 3) {
		sourceFormat = GL_RGB;
	}
	isLoadedFromFile = true;
}

void Texture::SelectToChange() {
	glBindTexture(type, textureId);
}

void Texture::Deselect() {
	glBindTexture(type, 0);
}

void Texture::SelectToRender(int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(type, textureId);
}

void Texture::Allocate() {
	SelectToChange();
	// set up the texture params
	glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
	// send texture to the gpu
	glTexImage2D(type, 0, internalFormat, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, textureData);
	Cleanup();
	glGenerateMipmap(GL_TEXTURE_2D);
	Deselect();
}

void Texture::Cleanup() {
	if (textureData == nullptr) {
		return;
	}
	if (!isLoadedFromFile) {
		delete[] textureData;
	}
	else {
		stbi_image_free(textureData);
	}
	textureData = nullptr;
}
