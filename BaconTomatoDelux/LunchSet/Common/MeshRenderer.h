#pragma once
#include "Component.h"
#include "IRender.h"
#include "../Graphics/Texture.h"

class MeshFilter;
class Material;

/**
 * \brief	: 정적 3d 모델을 렌더링하는 클래스
 */

class MeshRenderer : public Component, public IRender
{
public:
	MeshRenderer(std::weak_ptr<GameObject> owner);
	~MeshRenderer() override;

	void Initialize() override;
	void Start() override;
	void Render(Renderer* renderer) override;

public:
	// Component
	std::weak_ptr<MeshFilter> m_MeshFilter;

	// Object
	std::shared_ptr<Material> m_Material;
};
