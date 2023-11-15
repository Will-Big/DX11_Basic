#pragma once

class Object
{
public:
	Object() : m_Name(L"Default") {}
	Object(std::wstring_view name) : m_Name(name) {}
	virtual ~Object() = default;

protected:
	std::wstring m_Name;

public:
	const std::wstring& GetName() { return m_Name; }
};

