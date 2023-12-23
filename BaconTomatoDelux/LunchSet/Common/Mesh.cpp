#include "pch.h"
#include "Mesh.h"

#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexBuffer.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
{
	vertexBuffer = std::make_shared<VertexBuffer>();
	indexBuffer = std::make_shared<IndexBuffer>();
}

//Mesh::Mesh(ComPtr<ID3D11Device> device, std::wstring_view name)
//	: Object(name)
//{
//	vertexBuffer = std::make_shared<VertexBuffer>();
//	indexBuffer = std::make_shared<IndexBuffer>();
//}