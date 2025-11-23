#pragma once
#include "Core/ExcepAPI.h"
#include "World/WObject.h"
#include "Container/DynamicArray.h"
#include "Memory/UniquePtr.h"

// Forward declaration
namespace Excep
{
    class D3D11Renderer;
}

namespace Excep
{

/// @brief 모든 WObject를 관리하는 World 클래스
class EXCEP_API World
{
public:
    World();
    ~World() = default;

    // UniquePtr을 멤버로 가지므로 복사 불가
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

    /// @brief 새로운 WObject를 생성하여 World에 추가합니다
    /// @return 생성된 WObject 포인터
    WObject* SpawnObject();

    /// @brief 모든 오브젝트의 Update를 호출합니다
    void Update();

    /// @brief 모든 오브젝트를 렌더링합니다
    /// @param renderer D3D11Renderer 포인터
    void Render(D3D11Renderer* renderer);

    /// @brief 모든 오브젝트를 제거합니다
    void Clear();

    /// @brief World에 있는 오브젝트 개수를 반환합니다
    /// @return 오브젝트 개수
    uint64 GetObjectCount() const { return m_objects.GetSize(); }

    /// @brief 마지막에 추가된 오브젝트를 반환합니다
    /// @return 마지막 오브젝트 포인터, 오브젝트가 없으면 nullptr
    WObject* GetLastObject() const
    {
        if (m_objects.IsEmpty())
        {
            return nullptr;
        }
        return m_objects.GetBack().Get();
    }

private:
    #pragma warning(push)
    #pragma warning(disable: 4251)
    DynamicArray<UniquePtr<WObject>> m_objects;
    #pragma warning(pop)
};

} // namespace Excep
