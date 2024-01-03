#include "pch.h"
#include "GameProcess.h"

// Graphics
#include "../Graphics/ImGui_Initializer.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Renderer.h"

// Common
#include "GameProcess.h"

#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"

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

	// 멤버 변수 및 매니저 초기화
	m_Graphics = std::make_unique<Graphics>(m_hWnd, width, height, bImGuiInit);

	m_Renderer = std::make_unique<Renderer>(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());

	m_Time = std::make_unique<TimeManager>();
	m_Time->Initialize();

	ResourceManager::instance->Initialize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());

	InputManager::instance->Initialize(m_hWnd);
}

GameProcess::~GameProcess()
{
	// temp Scene delete
	m_GameObjects.clear();

	InputManager::instance->Finalize();
	ResourceManager::instance->Finalize();
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
	// system or manager update
	m_Time->Update();
	const float deltaTime = m_Time->GetDeltaTime();

	InputManager::instance->Update(deltaTime);

	// temp(Scene)
	{
		// Update
		for (auto& go : m_GameObjects)
		{
			go->Update(deltaTime);
		}

		// FixedUpdate
		for(auto& go : m_GameObjects)
		{
			go->FixedUpdate(deltaTime);
		}

		// LateUpdate
		for (auto& go : m_GameObjects)
		{
			go->LateUpdate(deltaTime);
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

		renderer->DrawAllQueue();
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
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}