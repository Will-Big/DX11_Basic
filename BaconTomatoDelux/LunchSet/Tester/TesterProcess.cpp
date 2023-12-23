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
#include "../Common/ResourceManager.h"
#include "../Common/ModelLoader.h"
#include "../Common/Transform.h"
#include "../Common/Camera.h"
#include "../Common/Light.h"
#include "../Common/Animator.h"


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
 */

static std::shared_ptr<GameObject> testGO;

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
	testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
	RES_MAN.GetModel(L"Primrose_Egypt", testGO);

	// Model Data Load 방식 2 (return)
	//testGO = RES_MAN.GetModel(L"Primrose_Egypt");
	//m_GameObjects.push_back(testGO);
	testGO->GetComponent<Transform>().lock()->SetPosition({0,0,0});
	testGO->GetComponent<Transform>().lock()->SetRenderGUI(true);

	// Animator test
	//testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/SkinningTest.fbx");

	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>().lock()->SetRenderGUI(true);
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 0.f, -200.f });

	m_GameObjects.emplace_back(GameObject::Create(L"Light"));
	m_GameObjects.back()->AddComponent<Light>().lock()->SetRenderGUI(true);

	// temp(Scene)
	for (auto& go : m_GameObjects)
		go->InitializeComponents();
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