#include "Core/Pch.h"
#include "World/World.h"
#include "World/CRenderer.h"
#include "Graphics/D3D11/D3D11Renderer.h"

namespace Excep
{
namespace World
{

World::World()
{
}

WObject* World::SpawnObject()
{
    Memory::UniquePtr<WObject> obj = Memory::MakeUnique<WObject>();
    WObject* ptr = obj.Get();
    m_objects.Add(std::move(obj));

    return ptr;
}

void World::Update()
{
    for (uint64 i = 0; i < m_objects.GetSize(); ++i)
    {
        m_objects[i]->Update();
    }
}

void World::Render(Graphics::D3D11Renderer* renderer)
{
    if (!renderer)
    {
        return;
    }

    // 렌더링 시작
    renderer->BeginRender();

    // 모든 오브젝트의 CRenderer 컴포넌트를 찾아서 Render() 호출
    for (uint64 i = 0; i < m_objects.GetSize(); ++i)
    {
        WObject* obj = m_objects[i].Get();

        // CRenderer 컴포넌트를 가져옵니다
        CRenderer* rendererComponent = obj->GetComponent<CRenderer>();
        if (rendererComponent)
        {
            rendererComponent->Render(renderer);
        }
    }
}

void World::Clear()
{
    m_objects.Clear();
}

} // namespace World
} // namespace Excep
