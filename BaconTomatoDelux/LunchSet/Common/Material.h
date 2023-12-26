#pragma once
#include "Object.h"
#include "../Graphics/Texture.h"

/**
 * \brief	: MeshRenderer 가 참조해야할 데이터를 가지고 있는 클래스
 *			  객체가 가르키는 그래픽 리소스는 공유자원으로 해당 객체를 사용하는 모든 인스턴스가 영향을 받음
 *			  이 텍스처를 사용하는 객체가 렌더링되기 위해서 필요한 그래픽 리소스를 가리키고 있음 - todo
 *					shader, input layout, texture 등
 */

class Material : public Object
{
public:
	Material(std::wstring_view name) : Object(name) {}
	~Material() override = default;

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

public:
	std::array<std::shared_ptr<Texture>, btdTextureType_END> textures;
};
