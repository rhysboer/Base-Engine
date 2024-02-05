#pragma once

namespace BE {
	template<class T>
	class Singleton
	{
	public:
		~Singleton() { delete instance; }
		inline T const* GetInstance() { return instance == nullptr ? instance = new T() : instance; }
	
	protected:
		Singleton() { }

	private:

		T* instance = nullptr;
	};
}

