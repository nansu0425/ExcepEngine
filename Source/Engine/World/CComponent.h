#pragma once
#include "Core/ExcepAPI.h"

namespace Excep
{

// Forward declaration
class WObject;

/// @brief 모든 컴포넌트의 기본 클래스
class EXCEP_API CComponent
{
public:
    CComponent();
    virtual ~CComponent() = default;

    /// @brief 매 프레임 호출되는 업데이트 함수
    virtual void Update() {}

    /// @brief 이 컴포넌트를 소유한 WObject를 반환합니다
    /// @return WObject 포인터
    WObject* GetOwner() const { return m_owner; }

    /// @brief 컴포넌트의 소유자를 설정합니다 (WObject에서 호출)
    /// @param owner 소유자 WObject
    void SetOwner(WObject* owner) { m_owner = owner; }

protected:
    WObject* m_owner;
};

} // namespace Excep
