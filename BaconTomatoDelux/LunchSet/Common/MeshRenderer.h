#pragma once
#include "Component.h"
#include "IRender.h"

class MeshFilter;
class Material;

/**
 * \brief	: 정적 3d 모델을 렌더링하는 클래스
 * \ref		: https://docs.unity3d.com/ScriptReference/MeshRenderer.html
 */

class MeshRenderer : public Component, public IRender
{
public:
	MeshRenderer(std::weak_ptr<GameObject> owner);
	~MeshRenderer() override;

	void Initialize() override;
	void Start() override;
	void Render(Renderer* renderer) override;
	void GUI() override;

public:
	std::weak_ptr<MeshFilter> m_MeshFilter;
	std::vector<std::shared_ptr<Material>> materials;
};
