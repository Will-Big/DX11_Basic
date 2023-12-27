﻿#pragma once
#include "Object.h"
#include "../Graphics/Texture.h"

/**
 * \brief	: 정점 정보와 인덱스 정보를 저장하는 클래스
 * \todo	: 템플릿으로 변경 해야함 -> VertexBuffer 만 template 으로 변경
 *			  Mesh 가 여러개라면 ?
 *				1. VertexBuffer, IndexBuffer 여러 종류를 Mesh 가 여러개 갖게 한다
 *				2. 새로운 Mesh 를 만든다
 * \ref		: https://docs.unity3d.com/ScriptReference/Mesh.html
 */

class VertexBuffer;
class IndexBuffer;

struct BoneReference
{
	std::string name;
	Matrix offset;
};

class Mesh// : public Object
{
public:
	Mesh(ComPtr<ID3D11Device> device);
	//Mesh(ComPtr<ID3D11Device> device, std::wstring_view name);
	~Mesh() = default;

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	// Bone
	std::vector<BoneReference> boneReferences;
};