#pragma once
#include "Core/Types.h"
#include <memory>
#include <utility>

namespace Excep
{
namespace Memory
{

/// @brief std::unique_ptr을 래핑한 독점 소유권 스마트 포인터
/// @tparam T 관리할 객체의 타입
/// @tparam Deleter 커스텀 삭제자 타입 (기본값: std::default_delete<T>)
template<typename T, typename Deleter = std::default_delete<T>>
class UniquePtr
{
public:
    /// @brief 기본 생성자 (nullptr로 초기화)
    UniquePtr()
        : m_ptr(nullptr)
    {
    }

    /// @brief nullptr 생성자
    UniquePtr(std::nullptr_t)
        : m_ptr(nullptr)
    {
    }

    /// @brief 원시 포인터로 생성
    /// @param ptr 관리할 원시 포인터
    explicit UniquePtr(T* ptr)
        : m_ptr(ptr)
    {
    }

    /// @brief 원시 포인터와 커스텀 삭제자로 생성
    /// @param ptr 관리할 원시 포인터
    /// @param deleter 커스텀 삭제자
    UniquePtr(T* ptr, const Deleter& deleter)
        : m_ptr(ptr, deleter)
    {
    }

    /// @brief 이동 생성자
    /// @param other 이동할 UniquePtr
    UniquePtr(UniquePtr&& other) noexcept
        : m_ptr(std::move(other.m_ptr))
    {
    }

    /// @brief std::unique_ptr로부터 이동 생성
    /// @param ptr 이동할 std::unique_ptr
    UniquePtr(std::unique_ptr<T, Deleter>&& ptr) noexcept
        : m_ptr(std::move(ptr))
    {
    }

    /// @brief 이동 대입 연산자
    /// @param other 이동할 UniquePtr
    /// @return 자기 자신의 참조
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        m_ptr = std::move(other.m_ptr);
        return *this;
    }

    /// @brief nullptr 대입
    /// @return 자기 자신의 참조
    UniquePtr& operator=(std::nullptr_t) noexcept
    {
        m_ptr.reset();
        return *this;
    }

    // 복사 생성자와 복사 대입 연산자 삭제 (이동 전용)
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    /// @brief 관리 중인 원시 포인터 반환
    /// @return 원시 포인터
    T* Get() const
    {
        return m_ptr.get();
    }

    /// @brief 소유권을 해제하고 원시 포인터 반환
    /// @return 원시 포인터
    T* Release()
    {
        return m_ptr.release();
    }

    /// @brief 포인터를 재설정
    /// @param ptr 새로운 포인터 (기본값: nullptr)
    void Reset(T* ptr = nullptr)
    {
        m_ptr.reset(ptr);
    }

    /// @brief 유효한 포인터를 가지고 있는지 확인
    /// @return 유효하면 true, nullptr이면 false
    bool8 IsValid() const
    {
        return m_ptr != nullptr;
    }

    /// @brief 역참조 연산자
    /// @return 관리 중인 객체의 참조
    T& operator*() const
    {
        return *m_ptr;
    }

    /// @brief 멤버 접근 연산자
    /// @return 관리 중인 객체의 포인터
    T* operator->() const
    {
        return m_ptr.get();
    }

    /// @brief bool 변환 연산자
    /// @return 유효하면 true, nullptr이면 false
    explicit operator bool() const
    {
        return m_ptr != nullptr;
    }

    /// @brief 내부 std::unique_ptr 반환 (이동)
    /// @return std::unique_ptr rvalue 참조
    std::unique_ptr<T, Deleter>&& GetStdUniquePtr() &&
    {
        return std::move(m_ptr);
    }

private:
    std::unique_ptr<T, Deleter> m_ptr;
};

} // namespace Memory
} // namespace Excep
