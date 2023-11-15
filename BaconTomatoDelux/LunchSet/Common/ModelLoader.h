#pragma once

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Material;
class GameObject;
class MeshRenderer;

struct ModelData
{
	std::wstring name;

	Vector3 scale = { 1.f, 1.f, 1.f };
	Quaternion rotation = { 0.f, 0.f, 0.f, 1.0f };
	Vector3 position = { 0.f, 0.f, 0.f };

	std::shared_ptr<Mesh> mesh;
	std::vector<ModelData> subs;
};

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ModelData& modelData);
	~ModelLoader();

	void Load(const std::string& path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, ModelData& modelData);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Mesh>& btdMesh);
	void ProcessAnimation(const aiScene* scene);

	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene, std::shared_ptr<Mesh>& mesh);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ModelData& m_RootData;
	std::wstring m_Path;
};

