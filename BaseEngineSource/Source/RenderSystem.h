#pragma once

namespace BE {
	class RenderSystem
	{
		friend class BaseEngine;
	public:

		~RenderSystem();


	private:
		RenderSystem() {}

		static void Render();
		static void RenderPass();
	};
}

