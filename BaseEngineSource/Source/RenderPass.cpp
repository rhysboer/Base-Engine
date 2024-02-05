#include "RenderPass.h"
#include "Logging.h"

#define CHECK_DEFAULT if (defaultTag == 0) RegisterDefaultTag()

namespace BE {
	std::unordered_map<std::string, PassTag> PassTags::tags = std::unordered_map<std::string, PassTag>();
	PassTag PassTags::defaultTag = 0;

	PassTag BE::PassTags::RegisterTag(const char* name)
	{
		CHECK_DEFAULT;

		BE_ASSERT(tags.size() < 32, "Too many registered passtags!");
		BE_ASSERT(tags.find(name) == tags.end(), "Pass with name (%s) already registered", name);

		PassTag tag = 1 << tags.size();
		tags.emplace(name, tag);
		return tag;
	}

	PassTag PassTags::GetTag(const char* name)
	{
		CHECK_DEFAULT;

		auto iter = tags.find(name);
		BE_ASSERT(iter != tags.end(), "Failed to find passtag (%s)", name);
		
		return (*iter).second;
	}

	PassTag PassTags::GetTags(std::vector<const char*> names)
	{
		CHECK_DEFAULT;

		int tag = 0;

		for (int i = 0; i < names.size(); i++)
		{
			tag |= GetTag(names[i]);
		}

		return tag;
	}

	PassTag PassTags::GetDefaultTag()
	{
		CHECK_DEFAULT;
		BE_ASSERT(defaultTag != 0, "Default tag does not exist");
		return defaultTag;
	}

	std::vector<std::string> PassTags::GetAllTags()
	{
		std::vector<std::string> tagList = std::vector<std::string>();
		auto iter = tags.begin();
		 
		while (iter != tags.end())
		{
			tagList.emplace_back((*iter).first);
			++iter;
		}

		return tagList;
	}

	void PassTags::RegisterDefaultTag()
	{
		tags.emplace(BE_PASSTAG_DEFAULT, 1);
		defaultTag = 1;
	}
}

