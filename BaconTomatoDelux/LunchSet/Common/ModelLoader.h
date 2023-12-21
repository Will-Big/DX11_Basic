#pragma once

#include <memory>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Graphics/VertexStruct.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"

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

	template<typename T>
	void Load(std::filesystem::path path);

private:
	template <typename T>
	void ProcessNode(aiNode* node, const aiScene* scene, ModelData& model);
	template <typename T>
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Mesh>& btdMesh);
	void ProcessAnimation(const aiScene* scene);

	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene, std::shared_ptr<Mesh>& mesh);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ModelData& m_RootData;
	std::filesystem::path m_folderPath;
};

template <typename T>
void ModelLoader::Load(std::filesystem::path path)
{
	m_folderPath = path;

	// warn : fbx 파일 이름이 폴더 이름과 같아야 함
	std::filesystem::path fbxPath = m_folderPath / m_folderPath.filename().replace_extension(".fbx");

	// 파일 유효성 검사
	if (!std::filesystem::exists(fbxPath) or fbxPath.extension() != L".fbx") {
		MessageBoxW(nullptr, L"FBX file does not exist", L"LOG_ERROR", MB_OK);
		return;
	}

	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함

	const aiScene* scene = importer.ReadFile(fbxPath.string(),
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded
	);

	if (scene == nullptr)
	{
		std::string errMsg = importer.GetErrorString();
		std::wstring wErrMsg(errMsg.begin(), errMsg.end());

		LOG_ERROR(wErrMsg.c_str());
		return;
	}

	ProcessNode<T>(scene->mRootNode, scene, m_RootData);
	ProcessAnimation(scene);
}

template <typename T>
void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, ModelData& model)
{
	std::string name = node->mName.C_Str();

	// 기본 데이터 입력
	model.name.assign(name.begin(), name.end());
	LOG_MESSAGE(model.name.c_str());

	aiVector3D scaling;
	aiQuaternion rotation;
	aiVector3D translation;

	node->mTransformation.Decompose(scaling, rotation, translation);

	model.scale = Vector3{ scaling.x, scaling.y, scaling.z };
	model.rotation = Quaternion{ rotation.x, rotation.y, rotation.z, rotation.w };
	model.position = Vector3{ translation.x, translation.y, translation.z };

	// 메시 데이터 입력
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh<T>(mesh, scene, model.mesh);
	}

	// 계층 구조 생성
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ModelData subModel{};
		ProcessNode<T>(node->mChildren[i], scene, subModel);
		model.subs.push_back(subModel);
	}
}

template <typename T>
void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Mesh>& btdMesh)
{
	if (btdMesh)
	{
		LOG_ERROR(L"Mesh already exists");
		return;
	}

	btdMesh = std::make_shared<Mesh>(m_Device);
	std::vector<T> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		T vertex{};

		if (mesh->HasPositions())
		{
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
			vertex.position.w = 1.0f;
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;
		}

		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			//if(std::is_same_v<ProfessorVertex, T>)
			//{
			//	vertex.bitangent.x = mesh->mBitangents[i].x;
			//	vertex.bitangent.y = mesh->mBitangents[i].y;
			//	vertex.bitangent.z = mesh->mBitangents[i].z;
			//}
		}

		vertices.push_back(std::move(vertex));
	}

	// Bone
	if constexpr (std::is_same_v<BoneVertex, T>)
	{
		if (mesh->HasBones())
		{
			btdMesh->boneReferences.resize(mesh->mNumBones);
			uint32_t boneIndexCounter = 0;
			std::map<std::string, int> boneMapping;

			for (uint32_t i = 0; i < mesh->mNumBones; ++i)
			{
				aiBone* bone = mesh->mBones[i];
				std::string boneName = bone->mName.C_Str();
				uint32_t boneIndex = 0;

				if(boneMapping.find(boneName) == boneMapping.end())
				{
					boneIndex = boneIndexCounter;
					boneIndexCounter++;
					btdMesh->boneReferences[boneIndex].name = boneName;
					btdMesh->boneReferences[boneIndex].offset = Matrix(&bone->mOffsetMatrix.a1).Transpose();
					boneMapping[boneName] = boneIndex;
				}
				else
				{
					boneIndex = boneMapping[boneName];
				}

				// bone 과 vertex 연결
				for(uint32_t j = 0; j < bone->mNumWeights; ++j)
				{
					uint32_t vertexID = bone->mWeights[j].mVertexId;
					float weight = bone->mWeights[j].mWeight;

					vertices[vertexID].AddBoneData(boneIndex, weight);
				}
			}
		}
	}

	btdMesh->vertexBuffer->Create(m_Device, vertices);

	indices.reserve(mesh->mNumFaces * 3);
	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(std::move(face.mIndices[j]));
	}
	btdMesh->indexBuffer->Create(m_Device, indices);

	static std::vector<aiTextureType> types =
	{
		aiTextureType_DIFFUSE,
		aiTextureType_NORMALS,
		aiTextureType_SPECULAR,
		aiTextureType_OPACITY,
		aiTextureType_EMISSIVE,
		aiTextureType_METALNESS,
		aiTextureType_SHININESS,
	};

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	for (const auto& type : types)
	{
		LoadMaterialTextures(material, type, scene, btdMesh);
	}
}
