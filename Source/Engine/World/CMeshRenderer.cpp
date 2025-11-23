#include "Core/Pch.h"
#include "World/CMeshRenderer.h"
#include "Graphics/D3D11/D3D11Renderer.h"
#include "World/WObject.h"
#include "World/CTransform.h"

namespace Excep
{

CMeshRenderer::CMeshRenderer()
    : m_meshType(MeshType::Triangle)
{
}

void CMeshRenderer::Render(D3D11Renderer* renderer)
{
    if (!renderer)
    {
        return;
    }

    // Transform은 Owner(WObject)가 소유하므로 GetOwner()->GetTransform()으로 접근
    renderer->RenderSingleMesh(m_meshType, GetOwner()->GetTransform()->GetPosition());
}

} // namespace Excep
