#pragma once

#include <imgui.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

class ImGui_Initializer
{
private:
	ImGui_Initializer() = default;
	~ImGui_Initializer() = default;

public:
	static bool Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
	static void RenderBegin();
	static void RenderEnd();
	static void Finalize();
};
