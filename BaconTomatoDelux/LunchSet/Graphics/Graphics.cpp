#include "pch.h"
#include "Graphics.h"

#include "ImGui_Initializer.h"

Graphics::Graphics(HWND hWnd, int width, int height, bool bImGui)
	: m_hWnd(hWnd), m_Width(width), m_Height(height), m_bImGuiInit(bImGui)
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	SetViewport();
	SetBlendState();
	InitializeDXGIAdapter();

	InitialSettings();

	// ImGUI
	if(m_bImGuiInit)
	{
		ImGui_Initializer::Initialize(m_hWnd, m_Device.Get(), m_DeviceContext.Get());
	}
}

Graphics::~Graphics()
{
	if (m_bImGuiInit)
	{
		ImGui_Initializer::Finalize();
	}
}

void Graphics::RenderBegin()
{
	// 필수 정보 설정
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), m_ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::RenderEnd()
{
	//auto check = m_Device->GetDeviceRemovedReason();
	HRESULT hr = m_SwapChain->Present(1, 0);
	HR_T(hr);
}

DXGI_QUERY_VIDEO_MEMORY_INFO Graphics::GetQueryVideoMemoryInfo()
{
	DXGI_QUERY_VIDEO_MEMORY_INFO memoryInfo = {};

	if (m_DXGIAdapter3) {
		m_DXGIAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memoryInfo);
	}

	return memoryInfo;
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	{
		desc.BufferDesc.Width = m_Width;
		desc.BufferDesc.Height = m_Height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = m_hWnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		m_SwapChain.GetAddressOf(),
		m_Device.GetAddressOf(),
		nullptr,
		m_DeviceContext.GetAddressOf()
	);
	HR_T(hr);
}

void Graphics::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC descTex = {};
	descTex.Width = m_Width;
	descTex.Height = m_Height;
	descTex.MipLevels = 1;
	descTex.ArraySize = 1;
	descTex.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descTex.SampleDesc.Count = 1;
	descTex.SampleDesc.Quality = 0;
	descTex.Usage = D3D11_USAGE_DEFAULT;
	descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descTex.CPUAccessFlags = 0;
	descTex.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureDepthStencil;
	HR_T(m_Device->CreateTexture2D(&descTex, nullptr, textureDepthStencil.GetAddressOf()));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(m_Device->CreateDepthStencilView(textureDepthStencil.Get(), &descDSV, m_DepthStencilView.GetAddressOf()));

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	//m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void Graphics::SetViewport()
{
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.Width = static_cast<float>(m_Width);
	m_Viewport.Height = static_cast<float>(m_Height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	HR_T(hr);

	hr = m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf());
	HR_T(hr);
}

void Graphics::SetBlendState()
{
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;						// 블렌드 사용 여부
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	    // SrcBlend는 SrcColor의 알파값
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend는 (1-SourceColor.a)
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;			// 컬러 블렌딩 연산을 정의합니다.
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;		// 알파 블렌딩 연산을 정의합니다.
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;		// SrcBlendAlpha = 1
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;		// DestBlendAlpha = 1	
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 렌더타겟에 RGBA 모두 Write	// 현재 렌더 타겟에 대한 컬러 컴포넌트 쓰기 마스크를 정의합니다.

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;  // 다중 샘플링 앤틸리어싱(Anti-aliasing)을 지원하기 위해 사용
	blendDesc.IndependentBlendEnable = false; // 각 렌더 타겟에 대한 독립적인 블렌딩 설정을 사용할지 여부
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_Device->CreateBlendState(&blendDesc, m_BlendState.GetAddressOf()));

	m_DeviceContext->OMSetBlendState(m_BlendState.Get(), nullptr, 0xffffffff);
}

void Graphics::InitializeDXGIAdapter()
{
	ComPtr<IDXGIDevice> dxgiDevice;
	m_Device.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	dxgiAdapter.As(&m_DXGIAdapter3);
}

void Graphics::InitialSettings()
{
	// RenderTarget 과 Viewport 는 단일로 가정
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}
