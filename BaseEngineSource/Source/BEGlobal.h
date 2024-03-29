#pragma once

#define BE_EVENT_WINDOW_RESIZE "WindowResize"
#define BE_EVENT_WINDOW_MINIMISE "WindowMinimise"
#define BE_EVENT_ENGINE_UPDATE "EngineUpdate"


#define BE_EPSILON 0.0000001f

namespace BE {
	class Texture;
	class Material;
	class BEGlobal
	{
		friend class BaseEngine;
	public:

		static inline unsigned int GetMaxTextureCount() { return maxTextureUnitCount; }
		static BE::Texture* GetDefaultTexture() { return defaultTexture; }
		static BE::Texture* GetDefaultNormalTexture() { return defaultNormal; }
		static BE::Material* GetDefaultMaterial() { return defaultMaterial; }


	private:
		BEGlobal() {}
		~BEGlobal() {}

		static void Init();

		static int maxTextureUnitCount;
		
		static BE::Texture* defaultTexture;
		static BE::Texture* defaultNormal;
		static BE::Material* defaultMaterial;
	};
}

