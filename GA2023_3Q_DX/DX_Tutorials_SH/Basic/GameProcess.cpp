#include "pch.h"
#include "GameProcess.h"

// System
#include "TimeSystem.h"

// WIN
#include "framework.h"
#include "resource.h"

#define USE_FLIPMODE 0	//경고 메세지를 없애려면 Flip 모드를 사용한다.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

WCHAR g_szClass[100] = L"Basic";
WCHAR g_szTitle[100] = L"Basic DX11 Viewer";

GameProcess::GameProcess(HINSTANCE hInstance)
{
	m_hInst = hInstance;

	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASIC));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	ATOM atom = RegisterClassExW(&wcex);
}

GameProcess::~GameProcess()
{
	FinalizeImGui();
	FinalizeScene();
	FinalizeD3D();
}

void GameProcess::Initialize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	RECT rect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	const int adjustedWidth = rect.right - rect.left;
	const int adjustedHeight = rect.bottom - rect.top;

	m_hWnd = CreateWindowW(g_szClass, g_szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, adjustedWidth, adjustedHeight, nullptr, nullptr, m_hInst, nullptr);

	assert(m_hWnd);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	// DX initialize
	InitializeD3D();

	// ImGui Initialize
	InitializeImGui();

	// Create Pipe Line
	InitializeScene();


	// System
	TimeSystem();
}

void GameProcess::Run()
{
	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}
}

void GameProcess::InitializeD3D()
{
	// 스왑체인 속성 설정 구조체 생성.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
#if USE_FLIPMODE == 1
	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#else
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//기본값 0
#endif
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;	// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = true;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = m_width;
	swapDesc.BufferDesc.Height = m_height;
	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// 샘플링 관련 설정.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	UINT creationFlags = 0;
#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapDesc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_context);
	HR_T(hr);

	// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
	HR_T(m_device->CreateRenderTargetView(pBackBufferTexture, NULL, &m_renderTargetView));  // 텍스처는 내부 참조 증가
	pBackBufferTexture->Release();	//외부 참조 카운트를 감소시킨다.

	D3D11_VIEWPORT viewport;
	ZERO(viewport);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &viewport);

#if USE_FLIPMODE == 0
	// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
	// FlipMode가 아닐때는 최초 한번만 설정하면 된다.
	m_context->OMSetRenderTargets(1, &m_renderTargetView, NULL);
#endif
	return;
}

void GameProcess::FinalizeD3D()
{
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_context);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_device);
}

void GameProcess::InitializeImGui()
{
	//ImGui 초기화. 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(this->m_device, this->m_context);
}

void GameProcess::FinalizeImGui()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GameProcess::Update()
{
	// System Update
	TimeSystem::m_Instance->Tick();

	auto t = TimeSystem::m_Instance->TotalTime();
	m_worlds[0] = XMMatrixRotationY(t);

	// 자식 큐브들의 월드 행렬 업데이트
	for (int i = 1; i < m_worlds.size(); ++i)
	{
		XMMATRIX mSpin = XMMatrixRotationZ(-t);
		XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
		XMMATRIX mTranslate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
		XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
		m_worlds[i] = mScale * mSpin * mTranslate * mOrbit;
	}
}

void GameProcess::Render()
{
#if USE_FLIPMODE==1
	// Flip모드에서는 매프레임 설정 필요
	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);
#endif


	// 화면 칠하기
	m_context->ClearRenderTargetView(m_renderTargetView, m_clearColor);
	//m_context->ClearDepthStencilView()

	// Draw 계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexBufferStride, &m_vertexBufferOffset);
	m_context->IASetInputLayout(m_inputLayout);
	m_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);
	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	// Draw
	ConstantBuffer cb;
	cb.view = XMMatrixTranspose(m_view);
	cb.projection = XMMatrixTranspose(m_projection);

	for (auto& cubeWorld : m_worlds)
	{
		cb.world = XMMatrixTranspose(cubeWorld);
		m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb, 0, 0);
		m_context->DrawIndexed(m_indices.size(), 0, 0);
	}

	// ImGui Render
	BeginRenderImGui();

	RenderImGui();

	EndRenderImGui();

	// 스왑체인 교체
	m_swapChain->Present(0, 0);
}

void GameProcess::RenderImGui()
{
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)	
		ImGui::ColorEdit3("clear color", (float*)&m_clearColor); // Edit 3 floats representing a color			
		ImGui::End();
	}
}

void GameProcess::BeginRenderImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GameProcess::EndRenderImGui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GameProcess::InitializeScene()
{
	CreateVertex();

	CreateVertexShader();

	CreatePixelShader();

	CreateConstantBuffer();
}

void GameProcess::FinalizeScene()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_inputLayout);
	SAFE_RELEASE(m_vertexShader);
	SAFE_RELEASE(m_pixelShader);
}

void GameProcess::CreateVertex()
{
	//1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼 및 버퍼 정보 준비
	// 아직은 VertexShader의 World, View, Projection 변환을 사용하지 않으므로 
	// 직접 Normalized Device Coordinate(좌표계)의 위치로 설정한다.
	//      /---------------------(1,1,1)   z값은 깊이값
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       중앙이 (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)

	m_vertices = {
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 1.0f) },
	};



	// 정점 정보
	D3D11_BUFFER_DESC vbDesc;
	ZERO(vbDesc);
	vbDesc.ByteWidth = sizeof(Vertex) * m_vertices.size();
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 정점 버퍼 생성
	D3D11_SUBRESOURCE_DATA vbData;
	ZERO(vbData);
	vbData.pSysMem = m_vertices.data();
	HR_T(m_device->CreateBuffer(&vbDesc, &vbData, &m_vertexBuffer));

	// 정점 버퍼 정보
	m_vertexBufferStride = sizeof(Vertex);
	m_vertexBufferOffset = 0;

	// 인덱스
	m_indices = {
		3,1,0, 2,1,3,
		0,5,4, 1,5,0,
		3,4,7, 0,4,3,
		1,6,5, 2,6,1,
		2,7,6, 3,7,2,
		6,4,5, 7,4,6,
	};

	// 인덱스 정보
	D3D11_BUFFER_DESC ibDesc;
	ZERO(ibDesc);
	ibDesc.ByteWidth = sizeof(WORD) * m_indices.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 버퍼 생성
	D3D11_SUBRESOURCE_DATA ibData;
	ZERO(ibData);
	ibData.pSysMem = m_indices.data();
	HR_T(m_device->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer));
}

void GameProcess::CreateVertexShader()
{
	// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 
	D3D11_INPUT_ELEMENT_DESC ieDesc[] = // 입력 레이아웃.
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },// 4byte * 3 = 12byte
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 대신 D3D11_APPEND_ALIGNED_ELEMENT 사용 가능.
	};

	ID3DBlob* vertexShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));

	HR_T(m_device->CreateInputLayout(
		ieDesc,
		ARRAYSIZE(ieDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_inputLayout));

	// 3. Render에서 파이프라인에 바인딩할 VertexShader 생성
	HR_T(m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader));
	SAFE_RELEASE(vertexShaderBuffer);
}

void GameProcess::CreatePixelShader()
{
	// 4. Render에서 파이프라인에 바인딩할 픽셀 셰이더 생성
	ID3DBlob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));

	HR_T(m_device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader));
	SAFE_RELEASE(pixelShaderBuffer);
}

void GameProcess::CreateConstantBuffer()
{
	// 6. Render() 에서 파이프라인에 바인딩할 상수 버퍼 생성
	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_device->CreateBuffer(&bd, nullptr, &m_constantBuffer));

	// 쉐이더에 전달할 데이터 설정
	// Initialize the world matrix

	// 부모
	m_worlds.push_back(XMMatrixIdentity());

	// 자식
	Matrix child1 = XMMatrixIdentity();
	child1.Translation({10.f, 0.f, 0.f});
	m_worlds.push_back(child1);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_view = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, m_width / (FLOAT)m_height, 0.01f, 100.0f);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
