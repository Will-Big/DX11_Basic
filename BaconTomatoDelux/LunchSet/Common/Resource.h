#pragma once
#include "Object.h"

class Resource : public Object
{
	friend class ResourceManager;

public:
	Resource(std::wstring_view name);
	~Resource() override;

protected:
	enum ResourceType
	{
		MODEL,
		TEXTURE,
		SAMPLER,
		SHADER,
		END,
	};

	static std::array<std::wstring, ResourceType::END> m_Directories;
};

