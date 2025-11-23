#pragma once
#include "Core/Types.h"
#include "Memory/SharedPtr.h"
#include <memory>
#include <utility>

namespace Excep
{

/// @brief std::weak_ptr을 래핑한 약한 참조 스마트 포인터
/// @tparam T 관리할 객체의 타입
template<typename T>
class WeakPtr
{
public:
    /// @brief 기본 생성자
    WeakPtr()
    {
    }

    /// @brief SharedPtr로부터 생성
    /// @param sharedPtr 참조할 SharedPtr
    WeakPtr(const SharedPtr<T>& sharedPtr)
        : m_ptr(sharedPtr.m_ptr)
    {
    }

    /// @brief 복사 생성자
    /// @param other 복사할 WeakPtr
    WeakPtr(const WeakPtr& other)
        : m_ptr(other.m_ptr)
    {
    }

    /// @brief 이동 생성자
    /// @param other 이동할 WeakPtr
    WeakPtr(WeakPtr&& other) noexcept
        : m_ptr(std::move(other.m_ptr))
    {
    }

    /// @brief std::weak_ptr로부터 생성
    /// @param ptr std::weak_ptr
    WeakPtr(const std::weak_ptr<T>& ptr)
        : m_ptr(ptr)
    {
    }

    /// @brief std::weak_ptr로부터 이동 생성
    /// @param ptr 이동할 std::weak_ptr
    WeakPtr(std::weak_ptr<T>&& ptr) noexcept
        : m_ptr(std::move(ptr))
    {
    }

    /// @brief 복사 대입 연산자
    /// @param other 복사할 WeakPtr
    /// @return 자기 자신의 참조
    WeakPtr& operator=(const WeakPtr& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }

    /// @brief 이동 대입 연산자
    /// @param other 이동할 WeakPtr
    /// @return 자기 자신의 참조
    WeakPtr& operator=(WeakPtr&& other) noexcept
    {
        m_ptr = std::move(other.m_ptr);
        return *this;
    }

    /// @brief SharedPtr 대입
    /// @param sharedPtr 참조할 SharedPtr
    /// @return 자기 자신의 참조
    WeakPtr& operator=(const SharedPtr<T>& sharedPtr)
    {
        m_ptr = sharedPtr.m_ptr;
        return *this;
    }

    /// @brief SharedPtr로 승격 (Lock)
    /// @return SharedPtr (만료되었으면 빈 SharedPtr)
    SharedPtr<T> Lock() const
    {
        return SharedPtr<T>(m_ptr.lock());
    }

    /// @brief 참조 해제
    void Reset()
    {
        m_ptr.reset();
    }

    /// @brief 참조 카운트 반환
    /// @return 현재 참조 카운트
    uint64 GetUseCount() const
    {
        return static_cast<uint64>(m_ptr.use_count());
    }

    /// @brief 만료 여부 확인
    /// @return 만료되었으면 true, 아니면 false
    bool8 IsExpired() const
    {
        return m_ptr.expired();
    }

    /// @brief 내부 std::weak_ptr 참조 반환 (const)
    /// @return std::weak_ptr const 참조
    const std::weak_ptr<T>& GetStdWeakPtr() const&
    {
        return m_ptr;
    }

    /// @brief 내부 std::weak_ptr 반환 (이동)
    /// @return std::weak_ptr rvalue 참조
    std::weak_ptr<T>&& GetStdWeakPtr() &&
    {
        return std::move(m_ptr);
    }

private:
    std::weak_ptr<T> m_ptr;
};

// SharedPtr의 WeakPtr 생성자 구현
template<typename T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& weakPtr)
    : m_ptr(weakPtr.m_ptr.lock())
{
}

} // namespace Excep
