#pragma once
#include "Component.h"
#include "IRender.h"

class MeshFilter;
class Material;
class Animator;

/**
 * \brief	: 애니메이션이 적용된 3d 모델을 렌더링하는 클래스
 */
class SkinnedMeshRenderer : public Component, public IRender
{
public:
	SkinnedMeshRenderer(std::weak_ptr<GameObject> owner);
	~SkinnedMeshRenderer() override;

	void Initialize() override;
	void Render(Renderer* renderer) override;

private:
	// Component
	std::weak_ptr<MeshFilter> m_MeshFilter;	// info : Unity 에서는 MeshFilter 를 사용하지 않음

	// Object
	std::shared_ptr<Material> m_Material;
};
