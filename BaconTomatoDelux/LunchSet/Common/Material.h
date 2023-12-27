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


/*
머티리얼이 사용할 쉐이더를 결정하는 방법은 여러 가지가 있습니다. 이는 게임 엔진이나 렌더링 시스템의 설계에 따라 다를 수 있지만,
일반적으로 다음과 같은 접근 방법을 사용할 수 있습니다:

1. 머티리얼 속성에 기반한 자동 결정 <- todo : 이 방식으로 변경 필요
	머티리얼의 특성(예: 반사율, 투명도, 텍스처 유무 등)에 기반하여 적절한 쉐이더를 자동으로 선택합니다.
	이 방법은 머티리얼에 설정된 특성을 분석하여, 미리 정의된 규칙에 따라 쉐이더를 결정합니다.
	예를 들어, 텍스처가 있는 머티리얼에는 텍스처를 지원하는 쉐이더를, 높은 반사율을 가진 머티리얼에는 반사 쉐이더를 사용하는 식입니다.
2. 머티리얼에 쉐이더 직접 할당 <- info : 일단은 이 방식
	머티리얼 생성 시, 개발자가 직접 쉐이더를 할당합니다.
	이 방법은 개발자가 머티리얼의 사용 목적과 필요한 렌더링 효과를 잘 알고 있을 때 유용합니다.
	예를 들어, 머티리얼을 만들 때 특정 쉐이더를 링크하거나 할당하는 방식입니다.
3. 머티리얼 타입과 쉐이더 매핑
	머티리얼 타입(예: 금속, 유리, 피부 등)과 쉐이더 간의 매핑 테이블을 사용합니다.
	이 테이블은 머티리얼 타입에 따라 사용할 쉐이더를 정의합니다.
	머티리얼이 생성될 때, 해당 타입에 맞는 쉐이더를 자동으로 선택합니다.
4. 스크립팅 및 메타데이터
	머티리얼의 메타데이터나 스크립트를 통해 쉐이더를 정의합니다.
	예를 들어, JSON, XML, 또는 다른 설정 파일에서 머티리얼과 관련 쉐이더 정보를 불러오는 방식입니다.
5. 런타임 결정
	렌더링 시, 머티리얼의 상태나 환경 조건에 따라 동적으로 쉐이더를 결정합니다.
	예를 들어, 조명 조건이나 카메라 위치에 따라 다른 쉐이더를 선택하는 것입니다.
	각 방법은 특정 상황과 요구 사항에 따라 장단점이 있으므로, 프로젝트의 목적과 구조에 맞게 적절한 방법을 선택하는 것이 중요합니다.
*/