#include "Engine/Components/CameraMoverComponent.h"

void CameraMover::Update() {
        auto &t = GetTransform();
        auto &input = InputManager::GetInstance();
        float dt = GetDeltaTime();
        
        // --- Look (right stick) - Do this FIRST ---
        glm::vec2 rightstick = input.GetStick(1);
        yaw += -rightstick.x * lookSpeed * dt;    // Removed the negative
        pitch += -rightstick.y * lookSpeed * dt; // Keep negative for natural feel
        
        // Clamp pitch
        float limit = glm::radians(89.0f);
        pitch = glm::clamp(pitch, -limit, limit);
        
        // Apply rotation - CORRECTED ORDER
        glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat yawQuat = glm::angleAxis(yaw, worldUp);
        glm::quat finalRotation = yawQuat * pitchQuat; // Yaw first, then pitch
        t.SetWorldRotation(finalRotation);
        
        // --- Movement (left stick) - Do this AFTER rotation update ---
        glm::vec2 leftstick = input.GetStick(0);

        bool leftTrigger = input.IsButtonDown(HidNpadButton_ZL);
        bool rightTrigger = input.IsButtonDown(HidNpadButton_ZR);
        if (leftTrigger) {
            //Move down
            t.SetWorldPosition(t.GetWorldPosition() - worldUp * moveSpeed * dt);
        }
        if (rightTrigger) {
            //Move up
            t.SetWorldPosition(t.GetWorldPosition() + worldUp * moveSpeed * dt);
        }

        // Use the transform's actual forward/right vectors
        glm::vec3 forward = t.GetForward();
        glm::vec3 right = t.GetRight();
        
        // Alternative if GetRight() doesn't exist:
        // glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
        
        glm::vec3 moveDelta = (forward * leftstick.y + right * -leftstick.x) * moveSpeed * dt;
        t.SetWorldPosition(t.GetWorldPosition() + moveDelta);
}