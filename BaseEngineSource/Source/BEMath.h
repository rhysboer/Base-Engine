#pragma once
#include <stdlib.h> 
#include "glm/vec3.hpp"

#define BE_PI 3.14159f
#define BE_INT_MAX 2147483647
#define BE_INT_MIN -2147483648
#define BE_ABS(x) (x < 0 ? x * -1 : x)

namespace BE::Math {
	inline bool IsBitSet(const int& value, const unsigned int& index) { return (value >> index) & 1; }
	inline int RandomInt(int min = 0, int max = BE_INT_MAX) { return (int)((rand() / (double)RAND_MAX) * (unsigned int)(max + -min + 1)) + min; }
	inline float RandomFloat(float min = 0.0f, float max = 1.0f) { return ((rand() / (double)RAND_MAX) * (double)(max + -min)) + min; }
}

