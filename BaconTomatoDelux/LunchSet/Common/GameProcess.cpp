#include "pch.h"
#include "GameProcess.h"

// Graphics
#include "../Graphics/ImGui_Initializer.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Renderer.h"

// Common
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"

uint32_t GameProcess::Width = 0;
uint32_t GameProcess::Height = 0;

std::vector<std::shared_ptr<GameObject>> GameProcess::m_GameObjects;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

GameProcess::GameProcess(HINSTANCE hInst, std::wstring_view title, int width, int height, bool bImGuiInit)
	: m_Title(title), m_bImGuiRender(bImGuiInit)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(292);

	GameProcess::Width = width;
	GameProcess::Height = height;

	// 윈도우 클래스 구조체 정의
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_Title.c_str();

	// 등록
	RegisterClass(&wndclass);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	m_hWnd = CreateWindowW(
		m_Title.c_str(),
		m_Title.c_str(),
		WS_OVERLAPPEDWINDOW,
		100, 100, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInst, NULL);

	assert(m_hWnd);

	// 윈도우 보이기
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	// 초기화
	m_Graphics = std::make_unique<Graphics>(m_hWnd, width, height, bImGuiInit);
	m_Renderer = std::make_unique<Renderer>(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());

	RES_MAN.Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());
	INPUT_MAN.Initialize();
}

GameProcess::~GameProcess()
{

}

void GameProcess::Loop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			DispatchMessage(&msg);
		}
		else
		{
			Update();

			m_Graphics->RenderBegin();

			Render(m_Renderer.get());
			ImGuiRender();

			m_Graphics->RenderEnd();
		}
	}
}

void GameProcess::Update()
{
	// system update
	INPUT_MAN.Update();

	// temp(Scene)
	{
		// Update
		for (auto& go : m_GameObjects)
		{
			go->Update(0.0167f);
		}

		// FixedUpdate
		for(auto& go : m_GameObjects)
		{
			go->FixedUpdate(0.0167f);
		}

		// LateUpdate
		for (auto& go : m_GameObjects)
		{
			go->LateUpdate(0.0167f);
		}
	}
}

void GameProcess::Render(Renderer* renderer)
{
	// temp(Scene)
	{
		// PreRender
		for (auto& go : m_GameObjects)
		{
			go->PreRender(renderer);
		}

		// Render
		for (auto& go : m_GameObjects)
		{
			go->Render(renderer);
		}
	}
}

void GameProcess::ImGuiRender()
{
	if (!m_bImGuiRender)
		return;

	ImGui_Initializer::RenderBegin();

	for (auto& go : m_GameObjects)
		go->GUI();

	ImGui_Initializer::RenderEnd();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return 0;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}