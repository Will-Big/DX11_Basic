﻿#pragma once

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN

// STL
#include <memory>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <algorithm>

// WIN
#include <windows.h>
#include <assert.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <directxtk/WICTextureLoader.h>
#include <dxgidebug.h>
#include <dxgi1_4.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")

// GUI
#include <imgui.h>

// USER
#include "../Graphics/Helper.h"

inline constexpr bool editor_mode = true;

#define CHECK_ENABLED \
	if constexpr (editor_mode) \
		if(!m_bEnabled) \
			return

#define ENABLE_CHECKBOX ImGui::Checkbox("Enabled", &m_bEnabled)

#endif //PCH_H
