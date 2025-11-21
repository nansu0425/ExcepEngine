#pragma once
#include "Core/ExcepAPI.h"
#include "World/CComponent.h"
#include "Math/Vector3.h"

namespace Excep
{
namespace World
{

/// @brief 위치, 회전, 스케일을 관리하는 Transform 컴포넌트
class EXCEP_API CTransform : public CComponent
{
public:
    CTransform();
    ~CTransform() override = default;

    /// @brief 위치를 반환합니다
    /// @return 위치 벡터
    const Math::Vector3& GetPosition() const { return m_position; }

    /// @brief 위치를 설정합니다
    /// @param position 새로운 위치
    void SetPosition(const Math::Vector3& position) { m_position = position; }

    /// @brief 회전을 반환합니다 (Euler angles)
    /// @return 회전 벡터
    const Math::Vector3& GetRotation() const { return m_rotation; }

    /// @brief 회전을 설정합니다 (Euler angles)
    /// @param rotation 새로운 회전
    void SetRotation(const Math::Vector3& rotation) { m_rotation = rotation; }

    /// @brief 스케일을 반환합니다
    /// @return 스케일 벡터
    const Math::Vector3& GetScale() const { return m_scale; }

    /// @brief 스케일을 설정합니다
    /// @param scale 새로운 스케일
    void SetScale(const Math::Vector3& scale) { m_scale = scale; }

private:
    #pragma warning(push)
    #pragma warning(disable: 4251)
    Math::Vector3 m_position;
    Math::Vector3 m_rotation;
    Math::Vector3 m_scale;
    #pragma warning(pop)
};

} // namespace World
} // namespace Excep
