#include "Core/Pch.h"
#include "Graphics/D3D11/D3D11Renderer.h"
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Excep
{
namespace Graphics
{

D3D11Renderer::D3D11Renderer()
    : m_width(0), m_height(0), m_sphereVertexCount(0)
{
    // Transform 초기화
    m_transformData.offset = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

D3D11Renderer::~D3D11Renderer()
{
    Shutdown();
}

bool8 D3D11Renderer::Initialize(HWND hwnd, int32 width, int32 height)
{
    m_width = width;
    m_height = height;

    if (!CreateDeviceAndSwapChain(hwnd, width, height))
    {
        return false;
    }

    if (!CreateRenderTargetView())
    {
        return false;
    }

    if (!CompileShaders())
    {
        return false;
    }

    if (!CreateInputLayout())
    {
        return false;
    }

    if (!CreateVertexBuffer())
    {
        return false;
    }

    if (!CreateCubeVertexBuffer())
    {
        return false;
    }

    if (!CreateSphereVertexBuffer())
    {
        return false;
    }

    if (!CreateConstantBuffer())
    {
        return false;
    }

    if (!CreateRasterizerState())
    {
        return false;
    }

    // 뷰포트 설정
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_deviceContext->RSSetViewports(1, &viewport);

    // Rasterizer State 설정
    m_deviceContext->RSSetState(m_rasterizerState.Get());

    return true;
}

void D3D11Renderer::Shutdown()
{
    m_rasterizerState.Reset();
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_vertexShader.Reset();
    m_constantBuffer.Reset();
    m_sphereVertexBuffer.Reset();
    m_cubeVertexBuffer.Reset();
    m_vertexBuffer.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_deviceContext.Reset();
    m_device.Reset();
}

void D3D11Renderer::Render()
{
    if (!m_deviceContext)
    {
        return;
    }

    // 백 버퍼 지우기
    float32 clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

    // 렌더 타겟 설정
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    // 셰이더 설정
    m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Constant Buffer 업데이트
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        memcpy(mappedResource.pData, &m_transformData, sizeof(TransformData));
        m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
    }

    // Constant Buffer를 Vertex Shader에 바인딩
    m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // 입력 레이아웃 설정
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());

    // 정점 버퍼 설정
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // 프리미티브 토폴로지 설정
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 삼각형 그리기
    m_deviceContext->Draw(3, 0);
}

void D3D11Renderer::Present()
{
    if (m_swapChain)
    {
        m_swapChain->Present(1, 0);
    }
}

void D3D11Renderer::OnResize(int32 width, int32 height)
{
    if (!m_device)
    {
        return;
    }

    m_width = width;
    m_height = height;

    m_renderTargetView.Reset();

    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    CreateRenderTargetView();

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_deviceContext->RSSetViewports(1, &viewport);
}

bool8 D3D11Renderer::CreateDeviceAndSwapChain(HWND hwnd, int32 width, int32 height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &featureLevel,
        m_deviceContext.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateVertexBuffer()
{
    // 삼각형 정점 (CCW 순서)
    Vertex vertices[] =
    {
        { Vector3(0.0f, 0.5f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },   // 상단 (빨강)
        { Vector3(-0.5f, -0.5f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) }, // 좌측 (파랑)
        { Vector3(0.5f, -0.5f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) }   // 우측 (초록)
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, &initData, m_vertexBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateCubeVertexBuffer()
{
    // DirectX Clip Space: Z는 0.0 ~ 1.0 범위여야 함
    // 큐브 정점 (36개 = 6면 × 6정점/면) - CCW 순서
    float32 size = 0.3f;
    Vertex vertices[] =
    {
        // Front face (Z=0.0) - 빨강 (near plane)
        { Vector3(-size, -size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(size, -size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(-size, -size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(-size, size, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

        // Back face (Z=0.6) - 초록 (far plane)
        { Vector3(size, -size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(-size, -size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(-size, size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, -size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(-size, size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.6f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },

        // Left face (X-) - 파랑
        { Vector3(-size, -size, 0.6f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, -size, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, size, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, -size, 0.6f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, size, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, size, 0.6f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

        // Right face (X+) - 노랑
        { Vector3(size, -size, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, -size, 0.6f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.6f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, -size, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.6f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(size, size, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },

        // Top face (Y+) - 자홍
        { Vector3(-size, size, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(size, size, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(size, size, 0.6f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, size, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(size, size, 0.6f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(-size, size, 0.6f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },

        // Bottom face (Y-) - 시안
        { Vector3(-size, -size, 0.6f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(size, -size, 0.6f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(size, -size, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-size, -size, 0.6f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(size, -size, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-size, -size, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, &initData, m_cubeVertexBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateSphereVertexBuffer()
{
    // UV Sphere 생성
    const float32 radius = 0.3f;
    const uint32 stacks = 16;  // 수평 분할
    const uint32 slices = 16;  // 수직 분할
    const float32 zOffset = 0.3f;  // DirectX Clip Space: Z를 0.0 ~ 1.0 범위로
    const float32 PI = 3.14159265359f;

    Container::DynamicArray<Vertex> vertices;

    // 각 스택과 슬라이스에 대해 삼각형 생성
    for (uint32 stack = 0; stack < stacks; ++stack)
    {
        float32 phi1 = PI * static_cast<float32>(stack) / static_cast<float32>(stacks);
        float32 phi2 = PI * static_cast<float32>(stack + 1) / static_cast<float32>(stacks);

        for (uint32 slice = 0; slice < slices; ++slice)
        {
            float32 theta1 = 2.0f * PI * static_cast<float32>(slice) / static_cast<float32>(slices);
            float32 theta2 = 2.0f * PI * static_cast<float32>(slice + 1) / static_cast<float32>(slices);

            // 4개의 정점 계산
            Vector3 v1(
                radius * sinf(phi1) * cosf(theta1),
                radius * cosf(phi1),
                radius * sinf(phi1) * sinf(theta1) + zOffset
            );
            Vector3 v2(
                radius * sinf(phi1) * cosf(theta2),
                radius * cosf(phi1),
                radius * sinf(phi1) * sinf(theta2) + zOffset
            );
            Vector3 v3(
                radius * sinf(phi2) * cosf(theta2),
                radius * cosf(phi2),
                radius * sinf(phi2) * sinf(theta2) + zOffset
            );
            Vector3 v4(
                radius * sinf(phi2) * cosf(theta1),
                radius * cosf(phi2),
                radius * sinf(phi2) * sinf(theta1) + zOffset
            );

            // 색상 (그라데이션)
            float32 colorFactor = static_cast<float32>(stack) / static_cast<float32>(stacks);
            Vector4 color(colorFactor, 0.5f, 1.0f - colorFactor, 1.0f);

            // 두 개의 삼각형으로 쿼드 구성 (CCW)
            if (stack != 0)  // 상단 극점 제외
            {
                vertices.Add({ v1, color });
                vertices.Add({ v2, color });
                vertices.Add({ v3, color });
            }

            if (stack != stacks - 1)  // 하단 극점 제외
            {
                vertices.Add({ v1, color });
                vertices.Add({ v3, color });
                vertices.Add({ v4, color });
            }
        }
    }

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<uint32>(vertices.GetSize() * sizeof(Vertex));
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.GetData();

    m_sphereVertexCount = static_cast<uint32>(vertices.GetSize());

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, &initData, m_sphereVertexBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(TransformData);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CompileShaders()
{
    // Vertex Shader 파일 읽기
    String8 vertexShaderSource;
    if (!ReadShaderFile(String16(L"Shaders/Default.vs.hlsl"), vertexShaderSource))
    {
        return false;
    }

    // Pixel Shader 파일 읽기
    String8 pixelShaderSource;
    if (!ReadShaderFile(String16(L"Shaders/Default.ps.hlsl"), pixelShaderSource))
    {
        return false;
    }

    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> errorBlob;

    // Vertex Shader 컴파일
    HRESULT hr = D3DCompile(
        vertexShaderSource.GetCString(),
        vertexShaderSource.GetLength(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        vsBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    hr = m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        m_vertexShader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    // Pixel Shader 컴파일
    ComPtr<ID3DBlob> psBlob;
    hr = D3DCompile(
        pixelShaderSource.GetCString(),
        pixelShaderSource.GetLength(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        psBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    hr = m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        return false;
    }

    // 입력 레이아웃 생성
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_device->CreateInputLayout(
        layout,
        ARRAYSIZE(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::CreateInputLayout()
{
    // 입력 레이아웃은 CompileShaders에서 생성됨
    return true;
}

bool8 D3D11Renderer::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = TRUE;  // CCW를 front face로 설정
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;

    HRESULT hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
    return SUCCEEDED(hr);
}

bool8 D3D11Renderer::ReadShaderFile(const String16& filename, String8& outSource)
{
    // 실행 파일의 경로 얻기
    char16 exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    // 실행 파일이 있는 디렉토리 추출
    String16 exeDir = exePath;
    uint64 lastSlash = exeDir.FindLastOf(String16(L"\\/"));
    if (lastSlash != UINT64_MAX)
    {
        exeDir = exeDir.Substring(0, lastSlash);
    }

    // 절대 경로 생성
    String16 fullPath = exeDir + String16(L"\\") + filename;

    std::ifstream file(fullPath.GetCString(), std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    outSource = String8(buffer.str());

    file.close();
    return true;
}

void D3D11Renderer::SetTriangleOffset(float32 x, float32 y)
{
    m_transformData.offset.x = x;
    m_transformData.offset.y = y;
    m_transformData.offset.z = 0.0f;
    m_transformData.offset.w = 0.0f;
}

void D3D11Renderer::RenderObjects(const Container::DynamicArray<SpawnedObject>& objects)
{
    if (!m_deviceContext || objects.IsEmpty())
    {
        // 배열이 비어있어도 화면은 클리어
        if (m_deviceContext)
        {
            float32 clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
            m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
            m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        }

        return;
    }

    // 1. 화면 클리어 (한 번만)
    float32 clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    // 2. 파이프라인 설정 (한 번만)
    m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;

    // 3. 각 오브젝트 렌더링
    for (uint64 i = 0; i < objects.GetSize(); ++i)
    {
        const SpawnedObject& obj = objects[i];

        // Constant Buffer 업데이트
        TransformData transformData;
        transformData.offset = Vector4(obj.position.x, obj.position.y, obj.position.z, 0.0f);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if (SUCCEEDED(m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
        {
            memcpy(mappedResource.pData, &transformData, sizeof(TransformData));
            m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
        }

        m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        // 타입에 따라 다른 버퍼와 정점 수 사용
        if (obj.type == MeshType::Triangle)
        {
            m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
            m_deviceContext->Draw(3, 0);
        }
        else if (obj.type == MeshType::Cube)
        {
            m_deviceContext->IASetVertexBuffers(0, 1, m_cubeVertexBuffer.GetAddressOf(), &stride, &offset);
            m_deviceContext->Draw(36, 0);
        }
        else if (obj.type == MeshType::Sphere)
        {
            m_deviceContext->IASetVertexBuffers(0, 1, m_sphereVertexBuffer.GetAddressOf(), &stride, &offset);
            m_deviceContext->Draw(m_sphereVertexCount, 0);
        }
    }
}

void D3D11Renderer::BeginRender()
{
    if (!m_deviceContext)
    {
        return;
    }

    // 1. 화면 클리어
    float32 clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    // 2. 파이프라인 설정
    m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Renderer::RenderSingleMesh(MeshType type, const Math::Vector3& position)
{
    if (!m_deviceContext)
    {
        return;
    }

    // Constant Buffer 업데이트
    TransformData transformData;
    transformData.offset = Vector4(position.x, position.y, position.z, 0.0f);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(m_deviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        memcpy(mappedResource.pData, &transformData, sizeof(TransformData));
        m_deviceContext->Unmap(m_constantBuffer.Get(), 0);
    }

    m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // 타입에 따라 다른 버퍼와 정점 수 사용
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;

    if (type == MeshType::Triangle)
    {
        m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_deviceContext->Draw(3, 0);
    }
    else if (type == MeshType::Cube)
    {
        m_deviceContext->IASetVertexBuffers(0, 1, m_cubeVertexBuffer.GetAddressOf(), &stride, &offset);
        m_deviceContext->Draw(36, 0);
    }
    else if (type == MeshType::Sphere)
    {
        m_deviceContext->IASetVertexBuffers(0, 1, m_sphereVertexBuffer.GetAddressOf(), &stride, &offset);
        m_deviceContext->Draw(m_sphereVertexCount, 0);
    }
}

} // namespace Graphics
} // namespace Excep
