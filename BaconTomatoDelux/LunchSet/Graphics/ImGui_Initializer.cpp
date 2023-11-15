#include "pch.h"
#include "ImGui_Initializer.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

bool ImGui_Initializer::Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	bool result = false;

	//ImGui √ ±‚»≠. 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends

	result = ImGui_ImplWin32_Init(hWnd);
	assert(result);

	result = ImGui_ImplDX11_Init(device, context);
	assert(result);

	return result;
}

void ImGui_Initializer::RenderBegin()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGui_Initializer::RenderEnd()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_Initializer::Finalize()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
