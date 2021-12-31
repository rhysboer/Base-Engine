#pragma once
#include "glm/mat4x4.hpp"
#include "Camera.h"

class Drawable {
public:
	// Renders an object
	virtual void Render(const glm::mat4& projectionView) = 0;
	virtual void Render(BE::Camera& camera) = 0;
	
	// Renders an object without setting any shader attributes
	virtual void RawRender() = 0;
};

