﻿#pragma once

#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif

#define CHECK(p) assert(SUCCEEDED(p));