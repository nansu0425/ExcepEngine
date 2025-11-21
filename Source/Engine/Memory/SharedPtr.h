#pragma once
#include "Core/Types.h"
#include <memory>
#include <utility>

namespace Excep
{
namespace Memory
{

// Forward declaration
template<typename T>
class WeakPtr;

/// @brief std::shared_ptr을 래핑한 공유 소유권 스마트 포인터
/// @tparam T 관리할 객체의 타입
template<typename T>
class SharedPtr
{
public:
    /// @brief 기본 생성자 (nullptr로 초기화)
    SharedPtr()
        : m_ptr(nullptr)
    {
    }

    /// @brief nullptr 생성자
    SharedPtr(std::nullptr_t)
        : m_ptr(nullptr)
    {
    }

    /// @brief 원시 포인터로 생성
    /// @param ptr 관리할 원시 포인터
    explicit SharedPtr(T* ptr)
        : m_ptr(ptr)
    {
    }

    /// @brief 복사 생성자
    /// @param other 복사할 SharedPtr
    SharedPtr(const SharedPtr& other)
        : m_ptr(other.m_ptr)
    {
    }

    /// @brief 이동 생성자
    /// @param other 이동할 SharedPtr
    SharedPtr(SharedPtr&& other) noexcept
        : m_ptr(std::move(other.m_ptr))
    {
    }

    /// @brief std::shared_ptr로부터 생성
    /// @param ptr std::shared_ptr
    SharedPtr(const std::shared_ptr<T>& ptr)
        : m_ptr(ptr)
    {
    }

    /// @brief std::shared_ptr로부터 이동 생성
    /// @param ptr 이동할 std::shared_ptr
    SharedPtr(std::shared_ptr<T>&& ptr) noexcept
        : m_ptr(std::move(ptr))
    {
    }

    /// @brief WeakPtr로부터 생성
    /// @param weakPtr WeakPtr
    explicit SharedPtr(const WeakPtr<T>& weakPtr);

    /// @brief 복사 대입 연산자
    /// @param other 복사할 SharedPtr
    /// @return 자기 자신의 참조
    SharedPtr& operator=(const SharedPtr& other)
    {
        m_ptr = other.m_ptr;
        return *this;
    }

    /// @brief 이동 대입 연산자
    /// @param other 이동할 SharedPtr
    /// @return 자기 자신의 참조
    SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        m_ptr = std::move(other.m_ptr);
        return *this;
    }

    /// @brief nullptr 대입
    /// @return 자기 자신의 참조
    SharedPtr& operator=(std::nullptr_t) noexcept
    {
        m_ptr.reset();
        return *this;
    }

    /// @brief 관리 중인 원시 포인터 반환
    /// @return 원시 포인터
    T* Get() const
    {
        return m_ptr.get();
    }

    /// @brief 포인터를 재설정
    /// @param ptr 새로운 포인터 (기본값: nullptr)
    void Reset(T* ptr = nullptr)
    {
        m_ptr.reset(ptr);
    }

    /// @brief 참조 카운트 반환
    /// @return 현재 참조 카운트
    uint64 GetUseCount() const
    {
        return static_cast<uint64>(m_ptr.use_count());
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

    /// @brief 내부 std::shared_ptr 참조 반환 (const)
    /// @return std::shared_ptr const 참조
    const std::shared_ptr<T>& GetStdSharedPtr() const&
    {
        return m_ptr;
    }

    /// @brief 내부 std::shared_ptr 반환 (이동)
    /// @return std::shared_ptr rvalue 참조
    std::shared_ptr<T>&& GetStdSharedPtr() &&
    {
        return std::move(m_ptr);
    }

private:
    std::shared_ptr<T> m_ptr;

    // WeakPtr이 내부 포인터에 접근할 수 있도록 friend 선언
    template<typename U>
    friend class WeakPtr;
};

} // namespace Memory
} // namespace Excep
