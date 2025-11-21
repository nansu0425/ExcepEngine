#pragma once
#include "Core/ExcepAPI.h"
#include "Container/StaticArray.h"

namespace Excep
{
namespace Input
{

/// @brief 키보드 입력을 관리하는 클래스
class EXCEP_API InputManager
{
public:
    InputManager();
    ~InputManager();

    /// @brief 프레임마다 호출하여 입력 상태를 업데이트합니다
    void Update();

    /// @brief 키보드 메시지를 처리합니다
    /// @param vkCode 가상 키 코드 (VK_*)
    /// @param isDown 키가 눌렸는지 여부
    void ProcessKeyboardMessage(uint32 vkCode, bool8 isDown);

    /// @brief 키가 현재 눌려있는지 확인합니다
    /// @param vkCode 가상 키 코드 (VK_*)
    /// @return 키가 눌려있으면 true
    bool8 IsKeyDown(uint32 vkCode) const;

    /// @brief 키가 이번 프레임에 눌렸는지 확인합니다 (눌림 순간만)
    /// @param vkCode 가상 키 코드 (VK_*)
    /// @return 키가 이번 프레임에 눌렸으면 true
    bool8 IsKeyPressed(uint32 vkCode) const;

    /// @brief 키가 이번 프레임에 떼어졌는지 확인합니다 (뗌 순간만)
    /// @param vkCode 가상 키 코드 (VK_*)
    /// @return 키가 이번 프레임에 떼어졌으면 true
    bool8 IsKeyReleased(uint32 vkCode) const;

private:
    static constexpr int32 KEY_COUNT = 256;

    #pragma warning(push)
    #pragma warning(disable: 4251)
    Container::StaticArray<bool8, KEY_COUNT> m_currentKeyState;
    Container::StaticArray<bool8, KEY_COUNT> m_previousKeyState;
    #pragma warning(pop)
};

} // namespace Input
} // namespace Excep
