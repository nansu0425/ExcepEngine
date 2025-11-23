#pragma once
#include "Core/ExcepAPI.h"
#include "World/CComponent.h"
#include "Container/DynamicArray.h"
#include "Memory/UniquePtr.h"
#include <typeinfo>

namespace Excep
{
namespace World
{

// Forward declaration
class CTransform;

/// @brief World에 존재하는 오브젝트의 기본 클래스
class EXCEP_API WObject
{
public:
    WObject();

    // UniquePtr을 멤버로 가지므로 복사 불가
    WObject(const WObject&) = delete;
    WObject& operator=(const WObject&) = delete;
    WObject(WObject&&) = delete;
    WObject& operator=(WObject&&) = delete;
    virtual ~WObject() = default;

    /// @brief 새로운 컴포넌트를 추가합니다
    /// @tparam T 추가할 컴포넌트 타입
    /// @return 추가된 컴포넌트 포인터
    template<typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of<CComponent, T>::value, "T must derive from CComponent");

        Memory::UniquePtr<T> component = Memory::MakeUnique<T>();
        T* ptr = component.Get();
        ptr->SetOwner(this);
        m_components.Add(Memory::UniquePtr<CComponent>(component.Release()));

        return ptr;
    }

    /// @brief 특정 타입의 컴포넌트를 반환합니다
    /// @tparam T 찾을 컴포넌트 타입
    /// @return 컴포넌트 포인터 (없으면 nullptr)
    template<typename T>
    T* GetComponent()
    {
        static_assert(std::is_base_of<CComponent, T>::value, "T must derive from CComponent");

        for (uint64 i = 0; i < m_components.GetSize(); ++i)
        {
            T* component = dynamic_cast<T*>(m_components[i].Get());
            if (component != nullptr)
            {
                return component;
            }
        }

        return nullptr;
    }

    /// @brief 모든 컴포넌트의 Update를 호출합니다
    void Update();

    /// @brief Transform 컴포넌트를 반환합니다
    /// @return Transform 컴포넌트 포인터 (모든 WObject는 Transform을 가짐)
    CTransform* GetTransform() const { return m_transform; }

private:
    #pragma warning(push)
    #pragma warning(disable: 4251)
    Container::DynamicArray<Memory::UniquePtr<CComponent>> m_components;
    #pragma warning(pop)

    CTransform* m_transform; // 빠른 접근을 위한 Transform 참조 (실제 소유권은 m_components에 있음)
};

} // namespace World
} // namespace Excep
