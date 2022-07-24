#pragma once
#define BE_PI 3.14159f

namespace BE::Math {
	inline bool IsBitSet(const int& value, const unsigned int& index) { return (value >> index) & 1; }
}

