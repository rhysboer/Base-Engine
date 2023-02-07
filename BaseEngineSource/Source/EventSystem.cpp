#include "EventSystem.h"
#include "Logging.h"

namespace BE {
	std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const void* const)>>> EventSystem::events = std::unordered_map<std::string, std::unordered_map<unsigned int, std::function<void(const void* const)>>>();
	unsigned int EventSystem::nextId = 0;

	unsigned int EventSystem::EventSubscribe(const char* eventName, std::function<void(const void* const)> onEvent)
	{
		auto iter = events.find(eventName);

		if (iter == events.end()) {
			BE_ERROR("EventSystem - Failed to find event named '%s'", eventName);
			return 0;
		}

		iter->second.emplace(++nextId, onEvent);
		return nextId;
	}

	void EventSystem::EventUnsubscribe(const char* eventName, const unsigned int& key)
	{
		auto iter = events.find(eventName);

		if (iter == events.end()) {
			BE_ERROR("EventSystem - Failed to find event named '%s'", eventName);
			return;
		}

		iter->second.erase(key);
	}

	void EventSystem::CreateEvent(const char* eventName)
	{
		if (events.find(eventName) != events.end()) {
			BE_ERROR("EventSystem - Event '%s' has already been created", eventName);
			return;
		}

		events.emplace(eventName, std::unordered_map<unsigned int, std::function<void(const void* const)>>());
	}

	void EventSystem::TriggerEvent(const char* eventName, const void* const data)
	{
		auto evnt = events.find(eventName);
		
		if (evnt == events.end()) {
			BE_ERROR("EventSystem - Failed to find event named '%s'", eventName);
			return;
		}
		
		auto iter = evnt->second.begin();
		while (iter != evnt->second.end()) {
			iter->second(data);
			iter++;
		}
	}
}