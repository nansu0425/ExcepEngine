#pragma once
#include "Core/ExcepAPI.h"
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
using Excep::Math::Vector3;
using Excep::Math::Vector4;

namespace Excep
{
namespace Graphics
{

struct Vertex
{
    Vector3 position;
    Vector4 color;
};

class EXCEP_API D3D11Renderer
{
public:
    D3D11Renderer();
    ~D3D11Renderer();

    bool8 Initialize(HWND hwnd, int32 width, int32 height);
    void Shutdown();
    void Render();
    void OnResize(int32 width, int32 height);

private:
    bool8 CreateDeviceAndSwapChain(HWND hwnd, int32 width, int32 height);
    bool8 CreateRenderTargetView();
    bool8 CreateVertexBuffer();
    bool8 CompileShaders();
    bool8 CreateInputLayout();
    bool8 CreateRasterizerState();
    bool8 ReadShaderFile(const std::wstring& filename, std::string& outSource);

    #pragma warning(push)
    #pragma warning(disable: 4251)  // ComPtr는 dll-interface가 필요하지 않음
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
    #pragma warning(pop)

    int32 m_width;
    int32 m_height;
};

} // namespace Graphics
} // namespace Excep
