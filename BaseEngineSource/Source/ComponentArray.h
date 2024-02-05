#pragma once
#include "IComponent.h"
#include <unordered_map>
#include <array>
#include "Logging.h"

#define MAX_COMPONENTS 128

namespace BE {
	class ComponentArray {
	public:
		ComponentArray() {};
		ComponentArray(IComponent* component) { Add(component); }
		~ComponentArray() {};

		/// <summary> Adds the component to the list, while returning the index </summary>
		void Add(IComponent* component);
		void Remove(IComponent* component);

		IComponent* AtIndex(const unsigned int& index) const;
		IComponent* GetFromEntityID(const unsigned int& entityID) const;
		inline bool Has(const unsigned int& entityID) const;
		inline int Size() const;

		IComponent* operator[] (const unsigned int& index) const;

	private:

		/// <summary> Holds the index for the component with the given Entity (Uint: EntityID, UInt: Component Index) </summary>
		std::unordered_map<unsigned int, unsigned int> indexes{};

		/// <summary> All components of the same type </summary>
		std::array<IComponent*, MAX_COMPONENTS> components{};

		unsigned int endPos = 0;
	};

	int ComponentArray::Size() const {
		return endPos;
	}

	inline IComponent* ComponentArray::operator[](const unsigned int& index) const
	{
		return components[index];
	}

	bool ComponentArray::Has(const unsigned int& entityID) const {
		return indexes.find(entityID) != indexes.end();
	}
}