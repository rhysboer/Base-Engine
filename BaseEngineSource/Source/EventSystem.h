#pragma once
#include <functional>
#include <unordered_map>

#define BE_EVENT_FUNC_CREATE(func) [=](const void* const d){ func(d); }

namespace BE {

	template<typename SIGNATURE, typename ... args>
	class Event {
		//void (SIGNATURE::* init)() = nullptr;
	};

	class EventSystem {
	public:

		static unsigned int EventSubscribe(const char* eventName, std::function<void(const void* const)> onEvent);
		static void EventUnsubscribe(const char* eventName, const unsigned int& key);
		
		static void CreateEvent(const char* eventName);

		static void TriggerEvent(const char* eventName, const void* const data = nullptr);

	private:

		EventSystem() {}
		~EventSystem() {}
		
		static unsigned int nextId;
		static std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const void* const)>>> events;
	};
}
