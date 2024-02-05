#pragma once

#define BE_GLOBALTEX_SHADOWDEPTH "BE_shadowDepthMap"

namespace BE {
	class Framebuffer;
	class Scene;
	class LightManager
	{
	public:
		LightManager(Scene* scene);
		~LightManager();

		void CreateShadowMap(const int& width, const int& height);
		void BindBuffer();

		Framebuffer* GetShadowMapFrameBuffer() const { return shadowBuffer; }
		inline int GetShadowMapWidth() const { return shadowWidth; }
		inline int GetShadowMapHeight() const { return shadowHeight; }

	protected:

		inline bool IsUsingShadowMap() const { return shadowBuffer != nullptr; }
		float ambientLight;

	private:

		Scene* scene;
		Framebuffer* shadowBuffer;
		int shadowWidth, shadowHeight;
		int countUniformIndex;
		int ambientUniformIndex;
	};
}

