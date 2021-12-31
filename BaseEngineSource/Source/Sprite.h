#pragma once
#include "BaseTexture.h"

namespace BE {
	class Sprite {//: public BaseTexture {
	public:

		Sprite();
		Sprite(const char* path);
		~Sprite();

		bool LoadTexture(const char* path);

	};
}
