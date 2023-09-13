#pragma once

class GameProcess
{
public:
	GameProcess(HINSTANCE hInstance);
	~GameProcess();

	void Initialize(unsigned int width, unsigned int height);
	void Run();

private:
	// Cycle
	void Update();
	void Render();

	void RenderImGui();
	void BeginRenderImGui();
	void EndRenderImGui();

	// Create for Rendering
	void InitializeD3D();
	void FinalizeD3D();

	void InitializeImGui();
	void FinalizeImGui();

	void InitializeScene();
	void FinalizeScene();

	void CreateVertex();
	void CreateVertexShader();
	void CreatePixelShader();
	void CreateConstantBuffer();

private:
	// WIN
	HINSTANCE m_hInst = nullptr;
	HWND m_hWnd = nullptr;

	unsigned int m_width;
	unsigned int m_height;

	Color m_clearColor = {};

	// DX
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

	// 쉐이더에 에 전달할 데이터
	// Cubes
	vector<Matrix> m_worlds;
	Matrix m_world;				// 월드좌표계 공간으로 변환을 위한 행렬.


	Matrix m_view;				// 카메라좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection;		// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	// IA
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ID3D11Buffer* m_constantBuffer = nullptr;

	UINT m_vertexBufferStride = 0;					// 버텍스 하나의 크기.
	UINT m_vertexBufferOffset = 0;					// 버텍스 버퍼의 오프셋.
	ID3D11InputLayout* m_inputLayout = nullptr;

	// VS
	ID3D11VertexShader* m_vertexShader = nullptr;

	// PS
	ID3D11PixelShader* m_pixelShader = nullptr;


};
