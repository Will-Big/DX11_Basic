#pragma once
#include "Object.h"

/**
 * \brief	: 정점 정보와 인덱스 정보를 저장하는 클래스
 * \todo	: ModelData 의 meshes 를 단일 메쉬로 변경
 * \ref		: https://docs.unity3d.com/ScriptReference/Mesh.html
 */

class VertexBuffer;
class IndexBuffer;

struct BoneReference
{
	std::string name;
	Matrix offset;
};

class Mesh : public Object
{
public:
	Mesh(ComPtr<ID3D11Device> device, std::wstring_view name);
	~Mesh() = default;

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::vector<BoneReference> boneReferences;
};