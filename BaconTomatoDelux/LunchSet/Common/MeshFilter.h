#pragma once
#include "Component.h"

class Mesh;

/**
 * \brief	: Mesh 를 참조하는 클래스
 * \ref		: https://docs.unity3d.com/ScriptReference/MeshFilter.html
 */

class MeshFilter : public Component
{
public:
	MeshFilter(std::weak_ptr<GameObject> owner);
	~MeshFilter() override = default;

public:
	std::vector<std::shared_ptr<Mesh>> meshes;
};
