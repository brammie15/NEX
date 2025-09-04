// InputManager.cpp
#include "InputManager.h"

InputManager::InputManager()
    : m_CurrentButtons(0), m_PreviousButtons(0) {
    padInitializeDefault(&m_PadState); // Initialize once
}

InputManager::~InputManager() {
}

bool InputManager::ProcessInput() {
    padUpdate(&m_PadState);

    u64 kDown = padGetButtonsDown(&m_PadState);
    u64 kHeld = padGetButtons(&m_PadState);

    // Store previous buttons
    m_PreviousButtons = m_CurrentButtons;

    // Update current buttons
    m_CurrentButtons = kHeld;

    // Quit if Plus pressed
    if (kDown & HidNpadButton_Plus) {
        return false;
    }

    return true;
}

bool InputManager::IsButtonDown(HidNpadButton buttonMask) {
    return (m_CurrentButtons & buttonMask) != 0;
}

bool InputManager::IsButtonUp(HidNpadButton buttonMask) {
    return (m_CurrentButtons & buttonMask) == 0;
}

bool InputManager::IsButtonPressed(HidNpadButton buttonMask) {
    return ((m_CurrentButtons & buttonMask) != 0) &&
           ((m_PreviousButtons & buttonMask) == 0);
}

glm::vec2 InputManager::GetStick(int stickIndex) {
    if (stickIndex == 0) { // Left stick
        HidAnalogStickState leftStick = padGetStickPos(&m_PadState, 0);
        return glm::vec2(leftStick.x / 32767.0f, leftStick.y / 32767.0f);
    } else if (stickIndex == 1) { // Right stick
        HidAnalogStickState rightStick = padGetStickPos(&m_PadState, 1);
        return glm::vec2(rightStick.x / 32767.0f, rightStick.y / 32767.0f);
    }
    return glm::vec2(0.0f, 0.0f); // Invalid stick index
}
