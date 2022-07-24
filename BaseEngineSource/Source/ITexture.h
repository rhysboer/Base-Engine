#pragma once
namespace BE {
	class ITexture
	{
	public:
		ITexture() {}
		virtual ~ITexture() = 0;

		void Bind() const;
		unsigned int GetActiveID() const;

	protected:
		
		void GenTextureID();

	private:
		
		unsigned int textureId = 0;
		mutable int activeIndex = -1;

		static inline int CreateActiveID() { nextActiveTexture++; return nextActiveTexture - 1; }
		static int nextActiveTexture;
	};
}

