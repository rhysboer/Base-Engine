#include "PhysicsManager.h"
#include "TimeManager.h"
#include "Scene.h"

BE::PhysicsManager* BE::PhysicsManager::instance = nullptr;

BE::PhysicsManager::PhysicsManager()
{
	instance = this;
	BE::Time::physicsTime = 1.0f / BE_DEFAULT_PHYSICS_UPDATES_PER_SECOND;
}

BE::PhysicsManager::~PhysicsManager()
{
	if (instance == this)
		instance = nullptr;
}

void BE::PhysicsManager::Start()
{
	lastUpdate = BE::Time::TotalTime();
}

void BE::PhysicsManager::Update()
{
	double time = BE::Time::TotalTime();
	float step = 1.0f / stepsPerSecond;
	while (lastUpdate < time) {
		Scene::PhysicsStepActiveScene(step);

		lastUpdate += step;
	}

	lastUpdate = BE::Time::TotalTime();
}

void BE::PhysicsManager::SetPhysicsUpdatesPerSecond(const unsigned int& time)
{
	stepsPerSecond = time;
	BE::Time::physicsTime = 1.0f / time;
}