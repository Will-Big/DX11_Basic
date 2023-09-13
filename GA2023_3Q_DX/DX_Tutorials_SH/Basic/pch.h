#pragma once

// USER
#include "Structs.h"
#include "Helper.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")

// ImGui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment (lib, "d3d11.lib")