// Basic.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "GameProcess.h"

#include <sstream>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
	GameProcess process(hInstance);

	try
	{
        process.Initialize(800, 600);

        process.Run();

        process.Finalize();
	}
    catch (const com_exception& e)
    {
        std::wstring errMsg = GetComErrorString(e.result);
        std::wstringstream ss;
        ss << L"Error : " << errMsg << L"\n";
        ss << L"File : " << GetRelativePath(std::wstring(e.file.begin(), e.file.end())) << L"\n";
        ss << L"Line : " << e.line;
        MessageBoxW(NULL, ss.str().c_str(), L"An error occurred", MB_OK | MB_ICONERROR);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), "An error occurred", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        // 알 수 없는 예외를 처리합니다.
        MessageBox(NULL, L"An unknown error occurred", L"Error", MB_OK | MB_ICONERROR);
    }

	return 1;
}
