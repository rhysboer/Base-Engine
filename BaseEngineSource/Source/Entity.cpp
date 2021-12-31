#include "Entity.h"
#include "Scene.h"
#include <assert.h>

namespace BE {
	unsigned int Entity::nextID = 0;

	Entity::Entity(Scene* scene, const char* name, glm::vec3 position) : scene(scene), transform(position), entityID(nextID)
	{ 
		this->name = (name == nullptr) ? "NewEntity_" + entityID : name;
		++nextID;
	}

	Entity::~Entity() { }
}