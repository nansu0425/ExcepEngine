#include <windows.h>
#include <memory>
#include "Graphics/D3D11/D3D11Renderer.h"

using Excep::Graphics::D3D11Renderer;

#define MAX_LOADSTRING 100

// 전역 변수
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"ExcepEngine Editor";
WCHAR szWindowClass[MAX_LOADSTRING] = L"EditorWindowClass";

std::unique_ptr<D3D11Renderer> g_renderer;
HWND g_hwnd = nullptr;

// 전방 선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg = {};

    // 메인 메시지 루프
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 프레임 렌더링
            if (g_renderer)
            {
                g_renderer->Render();
            }
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    int32 width = 800;
    int32 height = 600;

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!g_hwnd)
    {
        return FALSE;
    }

    // 렌더러 초기화
    g_renderer = std::make_unique<D3D11Renderer>();
    if (!g_renderer->Initialize(g_hwnd, width, height))
    {
        MessageBox(g_hwnd, L"DirectX 11 렌더러 초기화 실패!", L"오류", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    {
        if (g_renderer)
        {
            int32 width = LOWORD(lParam);
            int32 height = HIWORD(lParam);
            g_renderer->OnResize(width, height);
        }
    }
    break;

    case WM_DESTROY:
        g_renderer.reset();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
