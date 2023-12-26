#pragma once
#include "Object.h"
#include "../Graphics/Forward.h"

class Texture;
class Sampler;
class InputLayout;
class Shader;

/**
 * \brief	: MeshRenderer, SkinnedMeshRenderer 가 참조해야할 데이터를 가지고 있는 클래스
 *			  객체가 가르키는 그래픽 리소스는 공유자원으로 해당 객체를 사용하는 모든 인스턴스가 영향을 받음
 *			  이 텍스처를 사용하는 객체가 렌더링되기 위해서 필요한 그래픽 리소스를 가리키고 있음 - todo  start: 23.12.26. ~
 *					shader, input layout, texture, sampler 등
 * \ref		: https://docs.unity3d.com/ScriptReference/Material.html
 */

class Material : public Object
{
public:
	Material(std::wstring_view name) : Object(name) {}
	~Material() override = default;

	const std::shared_ptr<InputLayout>& GetInputLayout() { return inputLayout; }
	void SetInputLayout(std::shared_ptr<InputLayout> inputLayout) { this->inputLayout = inputLayout; }

	const std::shared_ptr<Sampler>& GetSampler() { return sampler; }
	void SetSampler(std::shared_ptr<Sampler> sampler) { this->sampler = sampler; }

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

	const std::shared_ptr<Shader>& GetShader(btdShaderScope scope)
	{
		assert(scope < btdTextureType_END);
		return shaders[scope];
	}

	void SetShader(btdShaderScope scope, std::shared_ptr<Shader> shader)
	{
		assert(scope < btdTextureType_END);
		shaders[scope] = shader;
	}

public:
	std::shared_ptr<InputLayout> inputLayout;
	std::shared_ptr<Sampler> sampler;		// warn : 일단은 따로 할당하지 않고 공용으로 사용
	std::array<std::shared_ptr<Shader>, btdShaderScope_END> shaders;
	std::array<std::shared_ptr<Texture>, btdTextureType_END> textures;
};
