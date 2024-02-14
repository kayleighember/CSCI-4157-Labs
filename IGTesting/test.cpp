#include "pch.h"
#include "..\KPInteractiveGraphicsS24\Texture.h"

namespace igtesting
{
	TEST(ATexture, HasNullTextureDataWhenCreated) {
		Texture sut;
		EXPECT_EQ(1, 1);
		EXPECT_TRUE(sut.GetTextureData() == nullptr);
	}
}
