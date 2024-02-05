#pragma once

#define BE_DEFAULT_PHYSICS_UPDATES_PER_SECOND 50

class BaseEngine;
namespace BE {
	class PhysicsManager
	{
		friend BaseEngine;
	public:
		PhysicsManager(PhysicsManager&) = delete;
		~PhysicsManager();

		void SetPhysicsUpdatesPerSecond(const unsigned int& time);
		inline unsigned int GetPhysicsUpdatesPerSecond() const { return stepsPerSecond; }

	private:
		PhysicsManager();

		static PhysicsManager* instance;

		void Start();
		void Update();

		unsigned int stepsPerSecond = BE_DEFAULT_PHYSICS_UPDATES_PER_SECOND;
		double lastUpdate = 0.0;
	};
}

