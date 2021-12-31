#pragma once
#include <functional>
#include <unordered_map>

namespace BE {
	class EventSystem {
	public:

		//static void Subscribe(const char* name, std::function<void> onEvent);
		//static void Unsubscribe(const char* name, std::function<void> onEvent);
		//
		//static void CreateEvent(const char* name);

	private:

		EventSystem();

		//static std::unordered_map<std::string, std::function<void>> events;
	};
}
