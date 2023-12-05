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

#include <random>

TesterProcess::TesterProcess(const HINSTANCE& hInst)
	: GameProcess(hInst, L"Tester Process", 800, 600, true)
{
	// Shader Compile

	// todo : shader, input layout, sampler 등 모두 공유자원으로 변경
	static auto vertexShader = std::make_shared<VertexShader>(m_Graphics->GetDevice());
	vertexShader->Create(L"../Resource/Shader/BoneVertexShader.hlsl", "main", "vs_5_0");

	static auto pixelShader = std::make_shared<PixelShader>(m_Graphics->GetDevice());
	pixelShader->Create(L"../Resource/Shader/LightPixelShader.hlsl", "main", "ps_5_0");

	static auto inputLayout = std::make_shared<InputLayout>(m_Graphics->GetDevice(), vertexShader->GetBlob());
	inputLayout->Create<BoneVertex>();

	static auto sampler = std::make_shared<Sampler>(m_Graphics->GetDevice());
	sampler->Create();

	m_Graphics->GetDeviceContext()->PSSetSamplers(0, 1, sampler->GetComPtr().GetAddressOf());
	m_Renderer->SetInputLayout(inputLayout); 
	m_Renderer->SetShader(vertexShader);
	m_Renderer->SetShader(pixelShader);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, 1000.0);

	for(int i = 0; i < 1; i++)
	{
		auto testGO = m_GameObjects.emplace_back(GameObject::Create(L"Test GO"));
		// Dummy_walker zeldaPosed001 BoxHuman SkinningTest
		RES_MAN.Load<ModelData, BoneVertex>(L"SkinningTest.fbx");
		RES_MAN.Get<ModelData>(L"SkinningTest.fbx", testGO);
		testGO->AddComponent<Animator>().lock()->SetController(L"../Resource/FBX/SkinningTest.fbx");

		Vector3 pos;
		pos.x = dis(gen);
		pos.y = dis(gen);
		pos.z = dis(gen);

		testGO->GetComponent<Transform>().lock()->SetPosition(pos);
	}


	m_GameObjects.emplace_back(GameObject::Create(L"Camera"));
	m_GameObjects.back()->AddComponent<Camera>();
	m_GameObjects.back()->GetComponent<Transform>().lock()->SetPosition({ 0.f, 70.f, -200.f });

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