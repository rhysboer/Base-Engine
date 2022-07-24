#include "BEGlobal.h"
#include "glad/glad.h"
#include "Texture.h"
#include "Material.h"
#include <vector>

#define WHITE 255, 255, 255, 255
#define GREY 128, 128, 128, 255
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
		  GREY, WHITE, GREY, WHITE,
		  WHITE, GREY, WHITE, GREY,
		  GREY, WHITE, GREY, WHITE,
		  WHITE, GREY, WHITE, GREY
		});
		std::vector<unsigned char> norm = std::vector<unsigned char>(
		{
			NORM, NORM, NORM, NORM,
		});

		// Generate Default Texture
		defaultTexture = new Texture(4, 4, tex);

		// Generate Default Normal Texture
		defaultNormal = new Texture(1, 1, norm);


		defaultMaterial = new Material("BE::Standard");

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnitCount);
		
	}
}

#undef WHITE
#undef GREY