#pragma once

namespace BE::Debug {
	class EntityDebugger
	{
	public:

		static void Update();

	private:
		EntityDebugger() { instance = this; }
		~EntityDebugger() {}

		static inline EntityDebugger* Get() { return (instance == nullptr) ? new EntityDebugger() : instance; }
		static EntityDebugger* instance;

		int selectedEntity = -1;
	};
}

