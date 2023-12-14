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
 *		- FSM 만들기
 *		- PBR 적용하기
 *			- Metalic, Roughness map 적용하기
 *			- 환경 조명 적용하기
 */

static std::shared_ptr<GameObject> testGO;

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 800, 600, true)
{
	// Shader Compile
	RES_MAN.LoadShader<VertexShader>("LightVertexShader.hlsl", "main", nullptr, L"LightVertexShader");
	RES_MAN.LoadShader<PixelShader>("LightPixelShader.hlsl", "main", nullptr, L"LightPixelShader");
	

	static auto inputLayout = std::make_shared<InputLayout>(m_Graphics->GetDevice(), RES_MAN.Get<VertexShader>(L"LightVertexShader")->GetBlob());
	inputLayout->Create<StaticVertex>();

	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());
	sampler->Create();

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());
	m_Renderer->SetInputLayout(inputLayout);
	m_Renderer->SetShader(RES_MAN.Get<VertexShader>(L"LightVertexShader"));
	m_Renderer->SetShader(RES_MAN.Get<PixelShader>(L"LightPixelShader"));

	testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
	// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
	// cerberus
	RES_MAN.LoadModel<BoneVertex>(L"cerberus", L"cerberus");
	RES_MAN.GetModel<ModelData>(L"cerberus", testGO);
	//testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/SkinningTest.fbx");

	testGO->GetComponent<Transform>().lock()->SetPosition({0,0,0});
	testGO->GetComponent<Transform>().lock()->SetLocalEulerRotation({0,90,0});


	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>();
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 0.f, -200.f });

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
	GameProcess::Update();
}

void TesterProcess::Render(Renderer* renderer)
{
	GameProcess::Render(renderer);
}