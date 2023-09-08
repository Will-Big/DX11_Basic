#pragma once
#include <d3dcompiler.h>
#include <wchar.h>
#include <exception>
#include <cstdio>

#pragma comment(lib, "d3dcompiler.lib")

#define LOG_ERROR(...) \
{ \
    wchar_t buffer[256]; \
    swprintf_s(buffer,256, L"[ERROR] %s:%d - ", __FUNCTIONW__, __LINE__); \
    wchar_t message[256]; \
    swprintf_s(message,256, __VA_ARGS__); \
    wcscat_s(buffer, message); \
    wcscat_s(buffer, L"\n"); \
    MessageBoxW(NULL, buffer, L"LOG_ERROR", MB_OK); \
}

#define LOG_WARNING(...) \
{ \
    wchar_t buffer[256]; \
    swprintf_s(buffer,256, L"[WARNING] %s:%d - ", __FUNCTIONW__, __LINE__); \
    wchar_t message[256]; \
    swprintf_s(message,256, __VA_ARGS__); \
    wcscat_s(buffer, message); \
    wcscat_s(buffer, L"\n"); \
    OutputDebugStringW(buffer); \
}

#define LOG_MESSAGE(...) \
{ \
    wchar_t buffer[256]; \
    swprintf_s(buffer,256, L"[MESSAGE] %s:%d - ", __FUNCTIONW__, __LINE__); \
    wchar_t message[256]; \
    swprintf_s(message,256, __VA_ARGS__); \
    wcscat_s(buffer, message); \
    wcscat_s(buffer, L"\n"); \
    OutputDebugStringW(buffer); \
}

#define ZERO(p) ZeroMemory((&(p)), sizeof(p));

#ifdef _DEBUG
	#include <iostream>
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
	#define DPRINT(MSG) printf(MSG)
#else
	#define DPRINT(MSG) __noop
#endif


template <typename T>
void SAFE_RELEASE(T* p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
void SAFE_DELETE(T* p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}


// For debug com_exception
LPCWSTR GetComErrorString(HRESULT hr);
std::wstring GetRelativePath(const std::wstring& fullPath);

// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr, const std::string& file, int line)
		: result(hr), file(file), line(line) {}

	const char* what() const noexcept override
	{
		static char s_str[64] = {};
		sprintf_s(s_str, "Failure with HRESULT of %08X",
			static_cast<unsigned int>(result));
		return s_str;
	}

	HRESULT result;
	std::string file;
	int line;
};

// Helper utility converts D3D API failures into exceptions.
//inline void HR_T(HRESULT hr)
//{
//	if (FAILED(hr))
//	{
//		throw com_exception(hr);
//	}
//}

#define HR_T(hr) \
    do { \
        HRESULT result = (hr); \
        if (FAILED(result)) { \
            throw com_exception(result, __FILE__, __LINE__); \
        } \
    } while (0)


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
