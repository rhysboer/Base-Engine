#pragma once

namespace BE {
	class Entity;
	class Transform;
	class IComponent {
		friend Entity;
	public:

		IComponent() {};

		/// <summary> Called on Component Construction </summary>
		virtual void OnStart() = 0;
		/// <summary> Called On Each Tick </summary>
		virtual void OnProcess() = 0;
		/// <summary> Called on Component Destruction </summary>
		virtual void OnDestroy() = 0;
		/// <summary> Returns the components unique ID </summary>
		virtual inline size_t GetID() const = 0;
		/// <summary> Returns the name of the component </summary>
		virtual inline const char* GetName() const = 0;
		/// <summary> Create IMGUI elements for BE Debugger (Optional) </summary>
		virtual void OnComponentDraw() {}

		bool IsActive();

		inline Entity* GetEntity() const { return entity; }
		Transform& GetTransform() const;

		template<typename T>
		inline static size_t GetComponentID() noexcept {
			static size_t typeID{ GenerateUniqueComponentID() };
			return typeID;
		}

	private:
		inline static size_t GenerateUniqueComponentID() noexcept {
			static size_t lastID{ 1u };
			return lastID++;
		}

		bool SetOwner(Entity* entity);

		bool isEnabled = true;

		Entity* entity = nullptr; 
	};
}

