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
#include "../Common/ResourceManager.h"
#include "../Common/ModelLoader.h"
#include "../Common/Transform.h"
#include "../Common/Camera.h"
#include "../Common/Light.h"
#include "../Common/Animator.h"

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 800, 600, true)
{
	// Shader Compile
	static auto vertexShader = std::make_shared<VertexShader>(m_Graphics->GetDevice());
	vertexShader->Create(L"../Resource/Shader/LightVertexShader.hlsl", "main", "vs_5_0");

	static auto pixelShader = std::make_shared<PixelShader>(m_Graphics->GetDevice());
	pixelShader->Create(L"../Resource/Shader/LightPixelShader.hlsl", "main", "ps_5_0");

	static auto inputLayout = std::make_shared<InputLayout>(m_Graphics->GetDevice(), vertexShader->GetBlob());
	inputLayout->Create<StaticVertex>();

	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());
	sampler->Create();

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());
	m_Renderer->SetInputLayout(inputLayout); 
	m_Renderer->SetShader(vertexShader);
	m_Renderer->SetShader(pixelShader);

	auto testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
	// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
	RES_MAN.Load<ModelData, StaticVertex>(L"../Resource/FBX/SkinningTest.fbx");
	RES_MAN.Get<ModelData>(L"../Resource/FBX/SkinningTest.fbx", testGO);
	//testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/Dummy_walker.fbx");


	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>();
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 400.f, -700.f });

	m_GameObjects.emplace_back(GameObject::Create(L"Light"));
	m_GameObjects.back()->AddComponent<Light>();

	// temp(Scene)
	for(auto& go : m_GameObjects)
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