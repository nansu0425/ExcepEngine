#pragma once
#include "Core/ExcepAPI.h"
#include "World/CRenderer.h"
#include "Graphics/D3D11/D3D11Renderer.h"

namespace Excep
{

/// @brief 메시를 렌더링하는 컴포넌트
class EXCEP_API CMeshRenderer : public CRenderer
{
public:
    CMeshRenderer();
    ~CMeshRenderer() override = default;

    /// @brief 메시 타입을 반환합니다
    /// @return 메시 타입
    MeshType GetMeshType() const { return m_meshType; }

    /// @brief 메시 타입을 설정합니다
    /// @param meshType 새로운 메시 타입
    void SetMeshType(MeshType meshType) { m_meshType = meshType; }

    /// @brief 렌더링을 수행합니다
    /// @param renderer D3D11Renderer 포인터
    void Render(D3D11Renderer* renderer) override;

private:
    MeshType m_meshType;
};

} // namespace Excep
