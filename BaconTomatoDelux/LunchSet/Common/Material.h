#pragma once
#include "Object.h"
#include "../Graphics/Texture.h"

/**
 * \brief	: Mesh 가 참조해야할 데이터를 가지고 있는 클래스
 */

class Material : public Object
{
public:
	Material(std::wstring_view name) : Object(name) {}
	~Material() override = default;

	std::shared_ptr<Texture>& operator[](std::wstring_view key) { return textures[key.data()]; }
	const std::shared_ptr<Texture>& operator[](std::wstring_view key) const { return textures.at(key.data()); }

public:
	std::map<std::wstring, std::shared_ptr<Texture>> textures;
};
