#pragma once
#include "BaseCamera.h"

class Camera2D : public BaseCamera {
public:

	Camera2D(const glm::vec3& _position, const glm::vec3& _lookAt, const float& left, const float& right, const float& bottom, const float& top, const float& near = 0.1f, const float& far = 1000.0f);

	void SetSize(const float& left, const float& right, const float& bottom, const float& top);
	void Zoom(const float& scale);

private:

	glm::vec3 lookAt;
	float left, right, top, bottom;
	float scale;

	// Inherited via BaseCamera
	virtual glm::mat4 CreateProjectionMatrix() const override;
	virtual void UpdateCamera() override;
};

