#pragma once
#include "Core/ExcepAPI.h"
#include "World/CTransform.h"

// Forward declaration
namespace Excep
{
namespace Graphics
{
    class D3D11Renderer;
}
}

namespace Excep
{
namespace World
{

/// @brief 렌더링을 담당하는 컴포넌트의 기본 클래스
/// @note Transform 기능을 포함하고 있으므로 CTransform을 상속받습니다
class EXCEP_API CRenderer : public CTransform
{
public:
    CRenderer();
    virtual ~CRenderer() = default;

    /// @brief 렌더링을 수행합니다
    /// @param renderer D3D11Renderer 포인터
    virtual void Render(Graphics::D3D11Renderer* renderer) = 0;
};

} // namespace World
} // namespace Excep
