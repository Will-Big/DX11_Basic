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
 *			- filesystem 사용하기(경로 단순화) - OK
 *		- Shader Macro 사용해서 오브젝트마다 다른 Shader 사용하기
 *		- FSM 만들기
 *		- PBR 적용하기
 *			- Metalic, Roughness map 적용하기
 *			- 환경 조명 적용하기
 */

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 800, 600, true)
{
	// Shader Compile
	static auto vertexShader = std::make_shared<VertexShader>(m_Graphics->GetDevice());
	vertexShader->Create(L"../Resource/Shader/BoneVertexShader.hlsl", "main", nullptr);

	RES_MAN.LoadShader<VertexShader>("BoneVertexShader.hlsl", "main", nullptr, L"BoneVertexShader");

	static auto pixelShader = std::make_shared<PixelShader>(m_Graphics->GetDevice());
	pixelShader->Create(L"../Resource/Shader/LightPixelShader.hlsl", "main", nullptr);

	static auto inputLayout = std::make_shared<InputLayout>(m_Graphics->GetDevice(), vertexShader->GetBlob());
	inputLayout->Create<BoneVertex>();

	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());
	sampler->Create();

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());
	m_Renderer->SetInputLayout(inputLayout);
	m_Renderer->SetShader(vertexShader);
	m_Renderer->SetShader(pixelShader);

	auto testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
	// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
	RES_MAN.LoadModel<BoneVertex>(L"SkinningTest.fbx", L"SkinningTest");
	RES_MAN.GetModel<ModelData>(L"SkinningTest", testGO);
	testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/SkinningTest.fbx");

	testGO->GetComponent<Transform>().lock()->SetPosition({0,0,0});


	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>();
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 70.f, -200.f });

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