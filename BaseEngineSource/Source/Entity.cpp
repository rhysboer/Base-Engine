#include "Entity.h"
#include "Scene.h"
#include <assert.h>

namespace BE {
	unsigned int Entity::nextID = 0;

	Entity::Entity(Scene* scene, const char* name, glm::vec3 position) : scene(scene), transform(position), entityID(nextID)
	{ 
		this->name = (name == nullptr) ? "NewEntity_" + std::to_string(entityID) : name;
		++nextID;
	}

	Entity::Entity(Scene* scene, const char* name, const float& x, const float& y, const float& z) : scene(scene), transform(x, y, z), entityID(nextID)
	{
		this->name = (name == nullptr) ? "NewEntity_" + std::to_string(entityID) : name;
		++nextID;
	}

	Entity::~Entity() { }
}