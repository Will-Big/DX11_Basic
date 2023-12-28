#pragma once
#include "Component.h"
#include "IRender.h"

class MeshFilter;
class Material;
class Animator;

/**
 * \brief	: 애니메이션이 적용된 3d 모델을 렌더링하는 클래스
 * \ref		: https://docs.unity3d.com/ScriptReference/SkinnedMeshRenderer.html
 */
class SkinnedMeshRenderer : public Component, public IRender
{
public:
	SkinnedMeshRenderer(std::weak_ptr<GameObject> owner);
	~SkinnedMeshRenderer() override;

	void Initialize() override;
	void Render(Renderer* renderer) override;

public:
	std::weak_ptr<MeshFilter> m_MeshFilter;
	std::vector<std::shared_ptr<Material>> materials;

private:
	std::vector<std::shared_ptr<Transform>> m_Bones;
};
