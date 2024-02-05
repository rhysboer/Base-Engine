#include "BEGlobal.h"
#include "glad/glad.h"
#include "Texture.h"
#include "Material.h"
#include <vector>

#define WHITE 255, 255, 255, 255
#define GREY 128, 128, 128, 255

#define WHITE_LINE WHITE, GREY, WHITE, GREY, WHITE, GREY, WHITE, GREY
#define GREY_LINE GREY, WHITE, GREY, WHITE, GREY, WHITE, GREY, WHITE

#define NORM 128, 128, 255, 255

namespace BE {
	int BEGlobal::maxTextureUnitCount = 0;

	Texture* BEGlobal::defaultTexture = nullptr;
	Texture* BEGlobal::defaultNormal = nullptr;
	Material* BEGlobal::defaultMaterial = nullptr;

	void BEGlobal::Init()
	{
		std::vector<unsigned char> tex = std::vector<unsigned char>(
		{
		  WHITE_LINE,
		  GREY_LINE,
		  WHITE_LINE,
		  GREY_LINE,
		  WHITE_LINE,
		  GREY_LINE,
		  WHITE_LINE,
		  GREY_LINE,
		  WHITE_LINE
		});

		std::vector<unsigned char> norm = std::vector<unsigned char>(
		{
			NORM, NORM, NORM, NORM,
		});

		TextureDesc desc = TextureDesc();
		desc.format = BE::TextureFormat::RGBA;
		desc.filter = BE::Filtering::NEAREST;

		defaultTexture = new Texture(8, 8, tex, desc); // Generate Default Texture
		defaultNormal = new Texture(1, 1, norm, desc); // Generate Default Normal Texture

		defaultMaterial = new Material("BE::Standard");

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnitCount);
		
	}
}

#undef WHITE
#undef GREY
#undef WHITE_LINE
#undef GREY_LINE