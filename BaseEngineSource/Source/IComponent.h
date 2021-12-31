#pragma once
//#define COMP_PRIORITY_DEFAULT 1000
//#define COMP_PRIORITY_RENDER 64000

namespace BE {
	class Entity;
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

		bool IsActive();

		inline Entity* GetEntity() const { return entity; }

		template<typename T>
		//requires(std::is_base_of_v<IComponent, T> && !std::is_same_v<IComponent, T>)
		inline static size_t GetComponentID() noexcept {
			static size_t typeID{ getUniqueComponentID() };
			return typeID;
		}

	private:
		inline static size_t getUniqueComponentID() noexcept {
			static size_t lastID{ 1u };
			return lastID++;
		}

		bool isEnabled = true;

		Entity* entity = nullptr; 
	};
}

