#pragma once
#include "Object.h"
#include "../Graphics/Texture.h"

/**
 * \brief	: MeshRenderer 가 참조해야할 데이터를 가지고 있는 클래스
 *			  여러개의 texture 로 조합된 textures 를 들고 있으며 언제든 구성품을 갈아낄 수 있음 (공유자원)
 * \warn	: 이 Material 클래스는 유니티와 모양이 다름. 현재 Mesh 가 들고있는 textures 가 이 Material 이 되어야 함.
 */

class Material : public Object
{
public:
	Material(std::wstring_view name) : Object(name) {}
	~Material() override = default;

//	std::shared_ptr<Texture>& operator[](std::wstring_view key) { return textureMap[key.data()]; }
//	const std::shared_ptr<Texture>& operator[](std::wstring_view key) const { return textureMap.at(key.data()); }
//
	const std::shared_ptr<Texture>& GetTexture(btdTextureType type)
	{
		assert(type < btdTextureType_END);
		return textures[type];
	}

	void SetTexture(btdTextureType type, std::shared_ptr<Texture> texture)
	{
		assert(type < btdTextureType_END);
		textures[type] = texture;
	}
//
//public:
//	std::map<std::wstring, std::shared_ptr<Texture>> textureMap;

public:
	std::array<std::shared_ptr<Texture>, btdTextureType_END> textures;
};
