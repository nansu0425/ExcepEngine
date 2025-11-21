#include "Core/Pch.h"
#include "World/CMeshRenderer.h"
#include "Graphics/D3D11/D3D11Renderer.h"

namespace Excep
{
namespace World
{

CMeshRenderer::CMeshRenderer()
    : m_meshType(Graphics::MeshType::Triangle)
{
}

void CMeshRenderer::Render(Graphics::D3D11Renderer* renderer)
{
    if (!renderer)
    {
        return;
    }

    // CRenderer는 CTransform을 상속받으므로 직접 GetPosition() 호출 가능
    renderer->RenderSingleMesh(m_meshType, GetPosition());
}

} // namespace World
} // namespace Excep
