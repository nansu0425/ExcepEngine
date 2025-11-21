#include "Core/Pch.h"
#include "Graphics/D3D11/D3D11Renderer.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// 간단한 Vertex Shader (HLSL)
const char* vertexShaderSource = R"(
struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}
)";

// 간단한 Pixel Shader (HLSL)
const char* pixelShaderSource = R"(
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
)";

D3D11Renderer::D3D11Renderer()
    : m_width(0), m_height(0)
{
}

D3D11Renderer::~D3D11Renderer()
{
    Shutdown();
}

bool D3D11Renderer::Initialize(HWND hwnd, int32 width, int32 height)
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

    // 백 버퍼 표시
    m_swapChain->Present(1, 0);
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

bool D3D11Renderer::CreateDeviceAndSwapChain(HWND hwnd, int32 width, int32 height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
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

bool D3D11Renderer::CreateRenderTargetView()
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

bool D3D11Renderer::CreateVertexBuffer()
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

bool D3D11Renderer::CompileShaders()
{
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> errorBlob;

    // Vertex Shader 컴파일
    HRESULT hr = D3DCompile(
        vertexShaderSource,
        strlen(vertexShaderSource),
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
        pixelShaderSource,
        strlen(pixelShaderSource),
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

bool D3D11Renderer::CreateInputLayout()
{
    // 입력 레이아웃은 CompileShaders에서 생성됨
    return true;
}

bool D3D11Renderer::CreateRasterizerState()
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
