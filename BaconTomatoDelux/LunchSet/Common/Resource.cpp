#include "pch.h"
#include "Resource.h"

std::array<std::wstring, Resource::ResourceType::END> Resource::m_Directories;

Resource::Resource(std::wstring_view name)
	: Object(name)
{
}

Resource::~Resource()
{
}



