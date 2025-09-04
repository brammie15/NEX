#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>

#include "../Singleton.h"

#include <switch.h>

#include <glm/glm.hpp>

class InputManager final: public Singleton<InputManager> {
public:
    InputManager();
    ~InputManager() override;

    bool IsButtonDown(HidNpadButton key);
    bool IsButtonUp(HidNpadButton key);
    bool IsButtonPressed(HidNpadButton key);

    glm::vec2 GetStick(int stickIndex); // 0 = left, 1 = right

    InputManager(const InputManager& other) = delete;
    InputManager(InputManager&& other) noexcept = delete;
    InputManager& operator=(const InputManager& other) = delete;
    InputManager& operator=(InputManager&& other) noexcept = delete;

    bool ProcessInput();

private:
    void HandleControllerInput();
    PadState m_PadState;
    u64 m_CurrentButtons;
    u64 m_PreviousButtons;
};