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
static size_t needInitIdx = 0;
static size_t constructCount = 0;
static size_t deleteCount = 0;

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 800, 600, true)
{
	// Shader Compile
	RES_MAN.LoadShader<VertexShader>("LightVertexShader.hlsl", "main", nullptr, L"PBR_VS");
	RES_MAN.LoadShader<PixelShader>("LightPixelShader.hlsl", "main", nullptr, L"PBR_PS");

	static auto inputLayout = std::make_shared<InputLayout>(m_Graphics->GetDevice(), RES_MAN.Get<VertexShader>(L"PBR_VS")->GetBlob());
	inputLayout->Create<StaticVertex>();

	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());
	m_Renderer->SetInputLayout(inputLayout);
	m_Renderer->SetShader(RES_MAN.Get<VertexShader>(L"PBR_VS"));
	m_Renderer->SetShader(RES_MAN.Get<PixelShader>(L"PBR_PS"));

	// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
	// PBR : cerberus Primrose_Egypt
	RES_MAN.LoadModel<StaticVertex>(L"Primrose_Egypt", L"Primrose_Egypt");

	// Model Data Load 방식 1 (reference)
	//testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
	//RES_MAN.GetModel(L"Primrose_Egypt", testGO);

	// Model Data Load 방식 2 (return)
	//testGO = RES_MAN.GetModel(L"Primrose_Egypt");
	//m_GameObjects.push_back(testGO);
	//testGO->GetComponent<Transform>().lock()->SetPosition({0,0,0});
	//testGO->GetComponent<Transform>().lock()->SetRenderGUI(true);

	// Animator test
	//testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/SkinningTest.fbx");

	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>().lock()->SetRenderGUI(true);
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 250.f, 250.f, -200.f });

	m_GameObjects.emplace_back(GameObject::Create(L"Light"));
	m_GameObjects.back()->AddComponent<Light>();

	// temp(Scene)
	for (auto& go : m_GameObjects)
		go->InitializeComponents();
}

TesterProcess::~TesterProcess()
{
}

void TesterProcess::Update()
{
	UpdateHW2();

	GameProcess::Update();
}

void TesterProcess::Render(Renderer* renderer)
{
	GameProcess::Render(renderer);
}

void TesterProcess::ImGuiRender()
{
	if (!m_bImGuiRender)
		return;

	ImGui_Initializer::RenderBegin();

	for (auto& go : m_GameObjects)
		go->GUI();

	ImGuiRenderHW2();

	ImGui_Initializer::RenderEnd();
}

void TesterProcess::UpdateHW2()
{
	// 무작위 숫자 생성을 위한 엔진과 분포 설정
	static std::random_device rd;
	static std::mt19937 rng(rd());
	static std::uniform_real_distribution<float> uni(0.f, 500.f);

	if (INPUT_MAN.IsKeyHold(VK_UP))
	{
		needInitIdx = m_GameObjects.size();
		std::wstring name = L"Primrose_Egypt" + std::to_wstring(constructCount);
		auto go = RES_MAN.GetModel(L"Primrose_Egypt", name);

		// 무작위 위치 생성
		float randomX = uni(rng);
		float randomY = uni(rng);
		float randomZ = uni(rng);

		go->GetComponent<Transform>().lock()->SetPosition({ randomX, randomY, randomZ });
		m_GameObjects.push_back(go);

		for (size_t i = needInitIdx; i < m_GameObjects.size(); i++)
		{
			m_GameObjects[i]->InitializeComponents();
		}

		constructCount++;
	}
	else if (INPUT_MAN.IsKeyHold(VK_DOWN))
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
		}
	}
}

void TesterProcess::ImGuiRenderHW2()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
	{
		ImGui::Text("Physical Memory Used: %llu MB", pmc.WorkingSetSize / (1024 * 1024)); // 현재 프로세스에 의해 사용되는 실제 메모리 (Working Set)
		ImGui::Text("Virtual Memory Used: %llu MB", pmc.PrivateUsage / (1024 * 1024));   // 현재 프로세스에 의해 사용되는 가상 메모리
	}
	else
	{
		ImGui::Text("Memory info not available");
	}
}
