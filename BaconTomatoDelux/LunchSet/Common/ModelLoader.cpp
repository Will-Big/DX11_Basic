#include "pch.h"
#include "ModelLoader.h"

#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "AnimatorController.h"
#include "Animator.h"
#include "AnimationClip.h"

// 함수 전방 선언
btdTextureType aiType2btdType(aiTextureType type);

ModelLoader::ModelLoader(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ModelData& modelData)
	: m_Device(device), m_DeviceContext(deviceContext), m_RootData(modelData)
{
}

ModelLoader::~ModelLoader()
{
}

//void ModelLoader::Load(const std::string& path)
//{
//	m_Path.assign(path.begin(), path.end());
//	Assimp::Importer importer;
//
//	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함
//
//	const aiScene* scene = importer.ReadFile(path,
//		aiProcess_Triangulate |
//		aiProcess_GenUVCoords |
//		aiProcess_GenNormals |
//		aiProcess_CalcTangentSpace |
//		aiProcess_ConvertToLeftHanded
//	);
//
//	if (scene == nullptr)
//	{
//		std::string errMsg = importer.GetErrorString();
//		std::wstring wErrMsg(errMsg.begin(), errMsg.end());
//
//		LOG_ERROR(wErrMsg.c_str());
//		return;
//	}
//
//	ProcessNode(scene->mRootNode, scene, m_RootData);
//	ProcessAnimation(scene);
//}
//
//void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, ModelData& model)
//{
//	std::string name = node->mName.C_Str();
//	LOG_MESSAGE(model.name.c_str());
//
//	// 기본 데이터 입력
//	model.name.assign(name.begin(), name.end());
//
//	aiVector3D scaling;
//	aiQuaternion rotation;
//	aiVector3D translation;
//
//	node->mTransformation.Decompose(scaling, rotation, translation);
//
//	model.scale = Vector3{ scaling.x, scaling.y, scaling.z };
//	model.rotation = Quaternion{ rotation.x, rotation.y, rotation.z, rotation.w };
//	model.position = Vector3{ translation.x, translation.y, translation.z };
//
//	// 메시 데이터 입력
//	for (uint32_t i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		ProcessMesh(mesh, scene, model.mesh);
//	}
//
//	// 계층 구조 생성
//	for (uint32_t i = 0; i < node->mNumChildren; i++)
//	{
//		ModelData subModel{};
//		ProcessNode(node->mChildren[i], scene, subModel);
//		model.subs.push_back(subModel);
//	}
//}
//
//void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<Mesh>& btdMesh)
//{
//	if (btdMesh)
//	{
//		LOG_ERROR(L"Mesh is already exist");
//		return;
//	}
//
//	btdMesh = std::make_shared<Mesh>(m_Device);
//	std::vector<StaticVertex> vertices;
//	std::vector<uint32_t> indices;
//
//	vertices.reserve(mesh->mNumVertices);
//	for (UINT i = 0; i < mesh->mNumVertices; i++) {
//		StaticVertex vertex;
//
//		if (mesh->HasPositions())
//		{
//			vertex.position.x = mesh->mVertices[i].x;
//			vertex.position.y = mesh->mVertices[i].y;
//			vertex.position.z = mesh->mVertices[i].z;
//			vertex.position.w = 1.0f;
//		}
//
//		if (mesh->HasTextureCoords(0))
//		{
//			vertex.uv.x = mesh->mTextureCoords[0][i].x;
//			vertex.uv.y = mesh->mTextureCoords[0][i].y;
//		}
//
//		if (mesh->HasNormals())
//		{
//			vertex.normal.x = mesh->mNormals[i].x;
//			vertex.normal.y = mesh->mNormals[i].y;
//			vertex.normal.z = mesh->mNormals[i].z;
//		}
//
//		if (mesh->HasTangentsAndBitangents())
//		{
//			vertex.tangent.x = mesh->mTangents[i].x;
//			vertex.tangent.y = mesh->mTangents[i].y;
//			vertex.tangent.z = mesh->mTangents[i].z;
//		}
//
//		if (mesh->HasBones())
//		{
//			//
//		}
//
//		vertices.emplace_back(vertex);
//	}
//	btdMesh->vertexBuffer->Create<StaticVertex>(m_Device, vertices);
//
//	indices.reserve(mesh->mNumFaces);
//	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
//		aiFace face = mesh->mFaces[i];
//
//		for (uint32_t j = 0; j < face.mNumIndices; j++)
//			indices.emplace_back(face.mIndices[j]);
//	}
//	btdMesh->indexBuffer->Create(m_Device, indices);
//
//	static std::vector<aiTextureType> types =
//	{
//		aiTextureType_DIFFUSE,
//		aiTextureType_NORMALS,
//		aiTextureType_SPECULAR,
//		aiTextureType_OPACITY,
//		aiTextureType_EMISSIVE,
//	};
//
//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//	for (auto& type : types)
//	{
//		LoadMaterialTextures(material, type, scene, btdMesh);
//	}
//}

void ModelLoader::ProcessAnimation(const aiScene* scene)
{
	// Animator Controller 중복 검사
	if (RES_MAN.animatorControllers.find(m_Path) != RES_MAN.animatorControllers.end())
	{
		LOG_ERROR(L"Animator Controller : Duplicated exist");
		return;
	}

	// todo : m_Path 로 animationController 를 생성하면 재활용이 불가능하므로 변경
	RES_MAN.animatorControllers[m_Path] = std::make_shared<AnimatorController>(m_Path);
	auto& ctrl = RES_MAN.animatorControllers[m_Path];


	for (uint32_t i = 0; i < scene->mNumAnimations; i++)
	{
		std::string animName(scene->mAnimations[i]->mName.C_Str());
		std::wstring wsAnimName{ animName.begin(), animName.end() };

		auto it = std::find_if(ctrl->animationClips.begin(), ctrl->animationClips.end(), [&wsAnimName](AnimationClip& clip)
			{
				return clip.GetName() == wsAnimName;
			});

		// AnimationClip 중복 검사
		if (it != ctrl->animationClips.end())
		{
			LOG_ERROR(L"AnimationClip : Duplicated exist");
			return;
		}
		auto& clip = ctrl->animationClips.emplace_back(wsAnimName);
		clip.duration = scene->mAnimations[i]->mDuration;
		clip.framePerSecond = scene->mAnimations[i]->mTicksPerSecond;
		clip.isLoop = true;	// todo : 삭제

		// Key Frame 생성
		for (uint32_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			auto nodeAnim = scene->mAnimations[i]->mChannels[j];
			std::string nodeName(nodeAnim->mNodeName.C_Str());
			std::wstring wsNodeName{ nodeName.begin(), nodeName.end() };

			KeyFrames frame;
			frame.name = wsNodeName;
			LOG_MESSAGE(wsNodeName.c_str());

			frame.scaleKeys.reserve(nodeAnim->mNumScalingKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumScalingKeys; k++)
			{
				auto sk = nodeAnim->mScalingKeys[k];
				frame.scaleKeys.emplace_back(sk.mTime, sk.mValue.x, sk.mValue.y, sk.mValue.z);
			}

			frame.rotationKeys.reserve(nodeAnim->mNumRotationKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumRotationKeys; k++)
			{
				auto rk = nodeAnim->mRotationKeys[k];
				frame.rotationKeys.emplace_back(rk.mTime, rk.mValue.x, rk.mValue.y, rk.mValue.z, rk.mValue.w);
			}

			frame.positionKeys.reserve(nodeAnim->mNumRotationKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumPositionKeys; k++)
			{
				auto pk = nodeAnim->mPositionKeys[k];
				frame.positionKeys.emplace_back(pk.mTime, pk.mValue.x, pk.mValue.y, pk.mValue.z);
			}

			clip.keyFrames.emplace_back(frame);
		}
	}
}


void ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene, std::shared_ptr<Mesh>& mesh)
{
	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string sTextureName = str.C_Str();
		std::wstring wsTextureName(sTextureName.begin(), sTextureName.end());
		btdTextureType btdType = aiType2btdType(type);

		// 전체 Material 중 key 확인
		auto materialTextures = RES_MAN.materials.find(m_Path);
		// Mesh 가 사용하는 Texture 참조 얻기
		auto& meshRefTexture = mesh->textures[btdType];

		if (materialTextures != RES_MAN.materials.end() && (*materialTextures->second)[wsTextureName])
			meshRefTexture = (*materialTextures->second)[wsTextureName];
		else
		{
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			std::shared_ptr<Texture> newTexture;

			if (embeddedTexture != nullptr)
				newTexture = std::make_shared<Texture>(LoadEmbeddedTexture(embeddedTexture), btdType);
			else
				newTexture = std::make_shared<Texture>(m_Device, wsTextureName, btdType);

			RES_MAN.materials[m_Path] = std::make_shared<Material>(m_Path);
			(*RES_MAN.materials[m_Path])[wsTextureName] = newTexture;
			meshRefTexture = newTexture;
		}

		LOG_MESSAGE(wsTextureName.c_str());
	}
}

ID3D11ShaderResourceView* ModelLoader::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData{};
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = m_Device->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			LOG_ERROR(L"CreateTexture2D failed!");

		hr = m_Device->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
			LOG_ERROR(L"CreateShaderResourceView failed!");

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(m_Device.Get(), m_DeviceContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
	if (FAILED(hr))
		LOG_ERROR(L"Texture couldn't be created from memory!");

	return texture;
}

btdTextureType aiType2btdType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		return btdTextureType_DIFFUSE;
	case aiTextureType_NORMALS:
		return btdTextureType_NORMALS;
	case aiTextureType_SPECULAR:
		return btdTextureType_SPECULAR;
	case aiTextureType_OPACITY:
		return btdTextureType_OPACITY;
	case aiTextureType_EMISSIVE:
		return btdTextureType_EMISSIVE;
	default:
		LOG_ERROR(L"Invalid Texture Type");
		return btdTextureType_END;
	}
}