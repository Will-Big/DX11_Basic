#pragma once

class GameProcess
{
public:
	GameProcess(HINSTANCE hInstance);
	~GameProcess();

	void Initialize(unsigned int width, unsigned int height);
	void Run();
	void Finalize();

private:
	// Cycle
	void Update();
	void Render();

	// Create for Rendering
	void InitializeD3D();
	void FinalizeD3D();
	void InitializeScene();
	void FinalizeScene();

	void CreateVertex();
	void CreateVertexShader();
	void CreatePixelShader();

private:
	// WIN
	HINSTANCE m_hInst = nullptr;
	HWND m_hWnd = nullptr;

	unsigned int m_width;
	unsigned int m_height;

	// DX
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

	// IA
	vector<Vertex> m_vertices;
	vector<size_t> m_indices;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	UINT m_vertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_vertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11InputLayout* m_inputLayout = nullptr;

	// VS
	ID3D11VertexShader* m_vertexShader = nullptr;

	// PS
	ID3D11PixelShader* m_pixelShader = nullptr;


};
