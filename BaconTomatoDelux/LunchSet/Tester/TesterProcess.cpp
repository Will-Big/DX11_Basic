#include "pch.h"
#include "TesterProcess.h"

#include "../Graphics/ImGui_Initializer.h"
#include "../Graphics/Graphics.h"

// Graphics Test
#include "../Graphics/VertexStruct.h"
#include "../Graphics/VertexShader.h"
#include "../Graphics/PixelShader.h"
#include "../Graphics/InputLayout.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Sampler.h"

// Common Test
#include "../Common/GameObject.h"
#include "../Common/InputManager.h"
#include "../Common/ResourceManager.h"
#include "../Common/ModelLoader.h"
#include "../Common/Transform.h"
#include "../Common/Camera.h"
#include "../Common/Light.h"
#include "../Common/Animator.h"
#include "../Common/Movement.h"

// Homework Test
#include <random>
#include <psapi.h>


/**
 * \todo list
 *		- Resource Manager 수정
 *			- 공유자원 추가하기
 *				- Shader, Input Layout, Sampler 등 ...
 *					- Shader - OK
 *					- InputLayout
 *					- Sampler
 *			- filesystem 사용하기(경로 단순화) - OK
 *		- Shader Macro 사용해서 오브젝트마다 다른 Shader 사용하기
 *			- 오브젝트 별로 Shader 를 런타임에 교체할 수 있도록 변경하기
 *		- FSM 만들기
 *		- PBR 적용하기
 *			- Metalic, Roughness map 적용하기 - OK
 *			- 환경 조명 적용하기
 *			- Bitangent 계산하지 않고 버텍스에 직접 넣기
 *		- 성능 표시하기
 *			- 메모리 - OK
 *			- FPS
 *		- Graphic Resource 교체하며 렌더링하기
 */

static std::shared_ptr<GameObject> testGO;

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 1024, 768, true)
{
	// Shader Load
	// Static
	ResourceManager::instance->LoadShader<VertexShader>("LightVertexShader.hlsl", "main", nullptr, L"PBR_VS");
	ResourceManager::instance->LoadShader<PixelShader>("LightPixelShader.hlsl", "main", nullptr, L"PBR_PS");

	// Skinned
	ResourceManager::instance->LoadShader<VertexShader>("BoneVertexShader.hlsl", "main", nullptr, L"SKIN_VS");

	// InputLayout Load
	// Static
	ResourceManager::instance->LoadInputLayout<StaticVertex>(L"PBR_VS", L"PBR_VS_INPUT");

	// Skinned
	ResourceManager::instance->LoadInputLayout<BoneVertex>(L"SKIN_VS", L"SKIN_VS_INPUT");

	// Sampler todo : ResourceManager 에서 Load 하게 변경
	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());

	// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
	// PBR : cerberus Primrose_Egypt

	auto staticInput = ResourceManager::instance->Get<InputLayout>(L"PBR_VS_INPUT");
	std::array<std::shared_ptr<Shader>, btdShaderScope_END> staticShaders
	{
		ResourceManager::instance->Get<VertexShader>(L"PBR_VS"),
		ResourceManager::instance->Get<PixelShader>(L"PBR_PS"),
	};
	ResourceManager::instance->LoadModel<StaticVertex>(L"Primrose_Egypt", L"Primrose_Egypt", staticShaders, staticInput);

	auto go = ResourceManager::instance->GetModel(L"Primrose_Egypt", L"Test GO1");

	if (go == nullptr)
		return;

	go->GetComponent<Transform>().lock()->SetPosition({ 0.f, 0, 0.f });
	m_GameObjects.push_back(go);

	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>();
	m_GameObjects.back()->AddComponent<Movement>();
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 100.f, -200.f });

	m_GameObjects.emplace_back(GameObject::Create(L"Light"));
	m_GameObjects.back()->AddComponent<Light>();

	// temp(Scene)
	for (auto& go : m_GameObjects)
		go->InitializeComponents();

	InputManager::instance->AddInputProcessor(this);
}

TesterProcess::~TesterProcess()
{
}

void TesterProcess::Update()
{
	GameProcess::Update();
}

void TesterProcess::Render(Renderer* renderer)
{
	GameProcess::Render(renderer);
}

void TesterProcess::ImGuiRender()
{
	GameProcess::ImGuiRender();

	//if (!m_bImGuiRender)
	//	return;

	//ImGui_Initializer::RenderBegin();

	//for (auto& go : m_GameObjects)
	//	go->GUI();

	//ImGuiRenderHW2();

	//ImGui_Initializer::RenderEnd();
}

// 무작위 숫자 생성을 위한 엔진과 분포 설정
static size_t objectNumber = -1;
static std::random_device rd;
static std::mt19937 rng(rd());
static std::uniform_real_distribution<float> uni(0.f, 500.f);

static int objectCount = 0;

void TesterProcess::UpdateHW2_Primrose(const InputStruct& input)
{
	if (objectNumber != 0)
		return;

	static size_t needInitIdx = 0;
	static size_t constructCount = 0;
	static size_t deleteCount = 0;

	if (input.keyState.IsKeyDown(Keyboard::Keys::Up))
	{
		needInitIdx = m_GameObjects.size();
		std::wstring name = L"Primrose_Egypt" + std::to_wstring(constructCount);
		auto go = ResourceManager::instance->GetModel(L"Primrose_Egypt", name);

		if (go == nullptr)
			return;

		// 무작위 위치 생성
		float randomX = uni(rng);

		go->GetComponent<Transform>().lock()->SetPosition({ randomX, 100.f, 0.f });
		m_GameObjects.push_back(go);

		for (size_t i = needInitIdx; i < m_GameObjects.size(); i++)
		{
			m_GameObjects[i]->InitializeComponents();
		}

		constructCount++;
		objectCount++;
	}
	else if (input.keyState.IsKeyDown(Keyboard::Keys::Down))
	{
		// 실제로 씬에서 적용할 때는 지우는 방식 수정 필요
		//		현재는 그냥 같은 root 를 가진 모든 오브젝트 삭제 방식
		//		
		std::wstring name = L"Primrose_Egypt" + std::to_wstring(deleteCount);

		auto target = std::find_if(m_GameObjects.begin(), m_GameObjects.end(),
			[&name](const std::shared_ptr<GameObject>& go) {
				return go->GetName() == name;
			});

		if (target != m_GameObjects.end())
		{
			auto rootTransform = (*target)->GetComponent<Transform>().lock();

			m_GameObjects.erase(std::remove_if(m_GameObjects.begin(), m_GameObjects.end(),
				[&](const std::shared_ptr<GameObject>& obj) {
					auto root = obj->GetComponent<Transform>().lock()->GetRoot().lock();
					return root == rootTransform;
				}),
				m_GameObjects.end());

			deleteCount++;
			objectCount--;
		}
	}
}

void TesterProcess::UpdateHW2_Skinned(const InputStruct& input)
{
	if (objectNumber != 1)
		return;

	static size_t needInitIdx = 0;
	static size_t constructCount = 0;
	static size_t deleteCount = 0;

	if (input.keyState.IsKeyDown(Keyboard::Keys::Up))
	{
		needInitIdx = m_GameObjects.size();
		std::wstring name = L"SkinningTest" + std::to_wstring(constructCount);
		auto go = ResourceManager::instance->GetModel(L"SkinningTest", name);

		if (go == nullptr)
			return;

		// 무작위 위치 생성
		float randomX = uni(rng);

		go->GetComponent<Transform>().lock()->SetPosition({ randomX, 300.f, 0.f });
		go->AddComponent<Animator>().lock()->SetController(L"../Resource/Model/SkinningTest");
		m_GameObjects.push_back(go);

		for (size_t i = needInitIdx; i < m_GameObjects.size(); i++)
		{
			m_GameObjects[i]->InitializeComponents();
		}

		constructCount++;
		objectCount++;
	}
	else if (input.keyState.IsKeyDown(Keyboard::Keys::Down))
	{
		std::wstring name = L"SkinningTest" + std::to_wstring(deleteCount);

		auto target = std::find_if(m_GameObjects.begin(), m_GameObjects.end(),
			[&name](const std::shared_ptr<GameObject>& go) {
				return go->GetName() == name;
			});

		if (target != m_GameObjects.end())
		{
			auto rootTransform = (*target)->GetComponent<Transform>().lock();

			m_GameObjects.erase(std::remove_if(m_GameObjects.begin(), m_GameObjects.end(),
				[&](const std::shared_ptr<GameObject>& obj) {
					auto root = obj->GetComponent<Transform>().lock()->GetRoot().lock();
					return root == rootTransform;
				}),
				m_GameObjects.end());

			deleteCount++;
			objectCount--;
		}
	}
}

void TesterProcess::ImGuiRenderHW2()
{
	//ImGui::Text("Up Arrow button   : Increase the object \nDown Arrow button : Decrease the object");
	//ImGui::Text("");

	//// 'Primrose_Egypt' 라디오 버튼
	//if (ImGui::RadioButton("Primrose_Egypt", objectNumber == 0)) {
	//	objectNumber = 0;
	//}

	//// 라디오 버튼 사이에 간격을 추가합니다.
	//ImGui::SameLine();

	//// 'SkinningTest' 라디오 버튼
	//if (ImGui::RadioButton("SkinningTest", objectNumber == 1)) {
	//	objectNumber = 1;
	//}

	//if (ImGui::Button("FBX Load")) 
	//{
	//	if(objectNumber == 0)
	//	{
	//		auto staticInput = ResourceManager::instance->Get<InputLayout>(L"PBR_VS_INPUT");
	//		std::array<std::shared_ptr<Shader>, btdShaderScope_END> staticShaders
	//		{
	//			ResourceManager::instance->Get<VertexShader>(L"PBR_VS"),
	//			ResourceManager::instance->Get<PixelShader>(L"PBR_PS"),
	//		};
	//		ResourceManager::instance->LoadModel<StaticVertex>(L"Primrose_Egypt", L"Primrose_Egypt", staticShaders, staticInput);
	//	}

	//	else if(objectNumber == 1)
	//	{
	//		auto skinnedInput = ResourceManager::instance->Get<InputLayout>(L"SKIN_VS_INPUT");
	//		std::array<std::shared_ptr<Shader>, btdShaderScope_END> boneShaders
	//		{
	//			ResourceManager::instance->Get<VertexShader>(L"SKIN_VS"),
	//			ResourceManager::instance->Get<PixelShader>(L"PBR_PS"),
	//		};
	//		ResourceManager::instance->LoadModel<BoneVertex>(L"SkinningTest", L"SkinningTest", boneShaders, skinnedInput);
	//	}
	//}

	//ImGui::Text("Object Count: %d", objectCount);
}

void TesterProcess::OnInputProcess(const InputStruct& input)
{
	//UpdateHW2_Primrose(input);
	//UpdateHW2_Skinned(input);
}
