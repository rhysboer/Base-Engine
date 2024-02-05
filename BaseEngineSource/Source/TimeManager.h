#pragma once

namespace BE {
	class PhysicsManager;
	class Time {
		friend PhysicsManager;
	public:
		static void Update();
		static float DeltaTime();
		static double TotalTime();
	private:
		static float deltaTime;
		static float sinceLastUpdate;
		static double totalTime;
		static float physicsTime;
	};
}
