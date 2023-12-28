#pragma once

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height, bool bImGui);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_DeviceContext; }
	DXGI_QUERY_VIDEO_MEMORY_INFO GetQueryVideoMemoryInfo();

private:
	void CreateDeviceAndSwapChain();
	void CreateDepthStencilView();
	void SetViewport();
	void CreateRenderTargetView();
	void SetBlendState();
	void InitializeDXGIAdapter();

	void InitialSettings();

private:
	// 윈도우 정보
	HWND m_hWnd = nullptr;
	int m_Width;
	int m_Height;
	float m_ClearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	D3D11_VIEWPORT m_Viewport = {};

	// ImGui 초기화 여부
	const bool m_bImGuiInit;

	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스 
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11BlendState> m_BlendState;

	// 디버그
	ComPtr<IDXGIAdapter3> m_DXGIAdapter3;
};