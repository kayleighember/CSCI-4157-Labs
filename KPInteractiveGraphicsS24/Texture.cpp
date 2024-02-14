#include "Texture.h"

Texture::Texture() {
	textureData = nullptr;
}

unsigned char* Texture::GetTextureData() {
	return textureData;
}