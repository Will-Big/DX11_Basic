#pragma once
#include "Component.h"

class Mesh;

/**
 * \brief	: Mesh 를 참조하는 클래스
 */

class MeshFilter : public Component
{
public:
	MeshFilter(std::weak_ptr<GameObject> owner);
	~MeshFilter() override;

public:
	std::shared_ptr<Mesh> mesh;
};

// https://docs.unity3d.com/kr/2021.3/Manual/m_MeshFilter.html