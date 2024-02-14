#pragma once
#include "BaseObject.h"
class Texture : public BaseObject
{
private:
	unsigned char* textureData;
public:
	Texture();
	~Texture() = default;
	unsigned char* GetTextureData();
};

