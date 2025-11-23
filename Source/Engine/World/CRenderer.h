#pragma once
#include "Core/ExcepAPI.h"
#include "World/CComponent.h"

// Forward declaration
namespace Excep
{
    class D3D11Renderer;
}

namespace Excep
{

/// @brief 렌더링을 담당하는 컴포넌트의 기본 클래스
/// @note Transform은 WObject가 소유하며, GetOwner()->GetTransform()으로 접근합니다
class EXCEP_API CRenderer : public CComponent
{
public:
    CRenderer();
    virtual ~CRenderer() = default;

    /// @brief 렌더링을 수행합니다
    /// @param renderer D3D11Renderer 포인터
    virtual void Render(D3D11Renderer* renderer) = 0;
};

} // namespace Excep
