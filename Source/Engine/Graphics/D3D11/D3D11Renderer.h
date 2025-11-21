#pragma once
#include "Core/ExcepAPI.h"
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
using Excep::Math::Vector3;
using Excep::Math::Vector4;
using Excep::Container::String8;
using Excep::Container::String16;

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

    /// @brief 렌더러를 초기화합니다
    /// @param hwnd 렌더링 대상 윈도우 핸들
    /// @param width 렌더링 해상도 너비
    /// @param height 렌더링 해상도 높이
    /// @return 초기화 성공 시 true, 실패 시 false
    bool8 Initialize(HWND hwnd, int32 width, int32 height);

    /// @brief 렌더러를 종료하고 모든 리소스를 해제합니다
    void Shutdown();

    /// @brief 한 프레임을 렌더링합니다 (Present 제외)
    void Render();

    /// @brief 백 버퍼를 화면에 표시합니다
    void Present();

    /// @brief 윈도우 크기 변경 시 렌더링 리소스를 재구성합니다
    /// @param width 새로운 너비
    /// @param height 새로운 높이
    void OnResize(int32 width, int32 height);

    /// @brief D3D11 Device를 반환합니다
    /// @return ID3D11Device 포인터
    ID3D11Device* GetDevice() const { return m_device.Get(); }

    /// @brief D3D11 Device Context를 반환합니다
    /// @return ID3D11DeviceContext 포인터
    ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext.Get(); }

    /// @brief Render Target View를 반환합니다
    /// @return ID3D11RenderTargetView 포인터
    ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }

private:
    bool8 CreateDeviceAndSwapChain(HWND hwnd, int32 width, int32 height);
    bool8 CreateRenderTargetView();
    bool8 CreateVertexBuffer();
    bool8 CompileShaders();
    bool8 CreateInputLayout();
    bool8 CreateRasterizerState();
    bool8 ReadShaderFile(const String16& filename, String8& outSource);

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
