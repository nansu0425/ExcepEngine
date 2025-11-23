#include "Core/Pch.h"
#include "Input/InputManager.h"

namespace Excep
{

InputManager::InputManager()
{
    m_currentKeyState.Fill(false);
    m_previousKeyState.Fill(false);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
    // 이전 프레임 상태를 현재 상태로 복사
    for (int32 i = 0; i < KEY_COUNT; ++i)
    {
        m_previousKeyState[i] = m_currentKeyState[i];
    }
}

void InputManager::ProcessKeyboardMessage(uint32 vkCode, bool8 isDown)
{
    if (vkCode < KEY_COUNT)
    {
        m_currentKeyState[vkCode] = isDown;
    }
}

bool8 InputManager::IsKeyDown(uint32 vkCode) const
{
    if (vkCode >= KEY_COUNT)
    {
        return false;
    }
    return m_currentKeyState[vkCode];
}

bool8 InputManager::IsKeyPressed(uint32 vkCode) const
{
    if (vkCode >= KEY_COUNT)
    {
        return false;
    }
    return m_currentKeyState[vkCode] && !m_previousKeyState[vkCode];
}

bool8 InputManager::IsKeyReleased(uint32 vkCode) const
{
    if (vkCode >= KEY_COUNT)
    {
        return false;
    }
    return !m_currentKeyState[vkCode] && m_previousKeyState[vkCode];
}

} // namespace Excep
