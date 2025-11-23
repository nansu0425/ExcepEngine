#include <windows.h>
#include "Graphics/D3D11/D3D11Renderer.h"
#include "Input/InputManager.h"
#include "Math/Vector3.h"
#include "Container/DynamicArray.h"
#include "World/World.h"
#include "World/WObject.h"
#include "World/CTransform.h"
#include "World/CMeshRenderer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

using namespace Excep;

#define MAX_LOADSTRING 100

// 전역 변수
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"ExcepEngine Editor";
WCHAR szWindowClass[MAX_LOADSTRING] = L"EditorWindowClass";

UniquePtr<D3D11Renderer> g_renderer;
UniquePtr<InputManager> g_inputManager;
HWND g_hwnd = nullptr;
bool8 g_isRunning = true;
UniquePtr<World> g_world;

// 전방 선언
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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
            if (g_renderer && g_isRunning)
            {
                // ImGui 프레임 시작
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                // 방향키 입력 처리 (ImGui가 키보드를 캡처하지 않을 때만)
                ImGuiIO& io = ImGui::GetIO();
                if (!io.WantCaptureKeyboard && g_world)
                {
                    WObject* lastObj = g_world->GetLastObject();
                    if (lastObj)
                    {
                        CMeshRenderer* meshRenderer = lastObj->GetComponent<CMeshRenderer>();
                        if (meshRenderer)
                        {
                            const float32 moveSpeed = 0.01f;
                            Vector3 pos = lastObj->GetTransform()->GetPosition();

                            if (g_inputManager->IsKeyDown(VK_LEFT))
                            {
                                pos.x -= moveSpeed;
                            }

                            if (g_inputManager->IsKeyDown(VK_RIGHT))
                            {
                                pos.x += moveSpeed;
                            }

                            if (g_inputManager->IsKeyDown(VK_UP))
                            {
                                pos.y += moveSpeed;
                            }

                            if (g_inputManager->IsKeyDown(VK_DOWN))
                            {
                                pos.y -= moveSpeed;
                            }

                            lastObj->GetTransform()->SetPosition(pos);
                        }
                    }
                }

                // UI 코드
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Exit"))
                        {
                            PostQuitMessage(0);
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMainMenuBar();
                }

                // Object Spawner UI
                static float32 spawnX = 0.0f;
                static float32 spawnY = 0.0f;
                static MeshType selectedType = MeshType::Triangle;

                ImGui::Begin("Object Spawner");

                // 메시 타입 선택
                ImGui::Text("Mesh Type:");
                ImGui::RadioButton("Triangle", (int*)&selectedType, (int)MeshType::Triangle);
                ImGui::SameLine();
                ImGui::RadioButton("Cube", (int*)&selectedType, (int)MeshType::Cube);
                ImGui::SameLine();
                ImGui::RadioButton("Sphere", (int*)&selectedType, (int)MeshType::Sphere);

                ImGui::Separator();
                ImGui::DragFloat("Spawn X", &spawnX, 0.01f, -1.0f, 1.0f);
                ImGui::DragFloat("Spawn Y", &spawnY, 0.01f, -1.0f, 1.0f);

                if (ImGui::Button("Spawn"))
                {
                    WObject* obj = g_world->SpawnObject();
                    CMeshRenderer* meshRenderer = obj->AddComponent<CMeshRenderer>();
                    meshRenderer->SetMeshType(selectedType);
                    obj->GetTransform()->SetPosition(Vector3(spawnX, spawnY, 0.0f));
                }

                ImGui::SameLine();
                if (ImGui::Button("Clear All"))
                {
                    g_world->Clear();
                }

                ImGui::Separator();
                ImGui::Text("Total Objects: %llu", g_world->GetObjectCount());

                WObject* lastObj = g_world->GetLastObject();
                if (lastObj)
                {
                    CMeshRenderer* meshRenderer = lastObj->GetComponent<CMeshRenderer>();

                    if (meshRenderer)
                    {
                        const char8* typeName = "Unknown";
                        MeshType type = meshRenderer->GetMeshType();
                        if (type == MeshType::Triangle)
                        {
                            typeName = "Triangle";
                        }
                        else if (type == MeshType::Cube)
                        {
                            typeName = "Cube";
                        }
                        else if (type == MeshType::Sphere)
                        {
                            typeName = "Sphere";
                        }

                        Vector3 pos = lastObj->GetTransform()->GetPosition();
                        ImGui::Text("Last Object: %s at (%.2f, %.2f)", typeName, pos.x, pos.y);
                    }
                }
                ImGui::End();

                // 1. Engine 렌더링 (Clear + Draw)
                g_world->Render(g_renderer.Get());

                // 2. ImGui 렌더링 (UI 오버레이)
                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                // 3. 화면에 표시
                g_renderer->Present();

                // 프레임 끝에 InputManager 업데이트 (다음 프레임 준비)
                g_inputManager->Update();
            }
        }
    }

    // 메인 루프 종료 후 정리
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    g_world.Reset();
    g_inputManager.Reset();
    g_renderer.Reset();

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
    g_renderer = MakeUnique<D3D11Renderer>();
    if (!g_renderer->Initialize(g_hwnd, width, height))
    {
        MessageBox(g_hwnd, L"DirectX 11 렌더러 초기화 실패!", L"오류", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    // InputManager 초기화
    g_inputManager = MakeUnique<InputManager>();

    // World 초기화
    g_world = MakeUnique<World>();

    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Multi-Viewport는 나중에 추가
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX11_Init(g_renderer->GetDevice(), g_renderer->GetDeviceContext());

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }

    switch (message)
    {
    case WM_KEYDOWN:
        if (g_inputManager)
        {
            g_inputManager->ProcessKeyboardMessage(static_cast<uint32>(wParam), true);
        }
        break;

    case WM_KEYUP:
        if (g_inputManager)
        {
            g_inputManager->ProcessKeyboardMessage(static_cast<uint32>(wParam), false);
        }
        break;

    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED)
            break;

        if (g_renderer)
        {
            int32 width = LOWORD(lParam);
            int32 height = HIWORD(lParam);
            g_renderer->OnResize(width, height);
        }
    }
    break;

    case WM_DESTROY:
        g_isRunning = false;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
