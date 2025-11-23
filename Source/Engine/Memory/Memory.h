#pragma once
#include "Memory/UniquePtr.h"
#include "Memory/SharedPtr.h"
#include "Memory/WeakPtr.h"
#include <memory>
#include <utility>

namespace Excep
{

/// @brief UniquePtr를 생성하는 헬퍼 함수
/// @tparam T 생성할 객체의 타입
/// @tparam Args 생성자 인자 타입들
/// @param args 생성자 인자들
/// @return 생성된 UniquePtr
template<typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

/// @brief SharedPtr를 생성하는 헬퍼 함수
/// @tparam T 생성할 객체의 타입
/// @tparam Args 생성자 인자 타입들
/// @param args 생성자 인자들
/// @return 생성된 SharedPtr
template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
    return SharedPtr<T>(std::make_shared<T>(std::forward<Args>(args)...));
}

} // namespace Excep
