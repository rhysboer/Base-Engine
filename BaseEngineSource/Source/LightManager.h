#pragma once


namespace BE {
	class Scene;
	class LightManager
	{
	public:
		LightManager(Scene* scene);
		~LightManager() {};

		void BindLights();

	protected:

		float ambientLight;

	private:

		Scene* scene;
		int countUniformIndex;
		int ambientUniformIndex;
	};
}

