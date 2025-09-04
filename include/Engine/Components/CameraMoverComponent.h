#ifndef CAMERA_MOVER_H
#define CAMERA_MOVER_H
#include "Component.h"
#include "InputManager.h"
#include "Transform.h"
#include "DeltaTime.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

class CameraMover : public Component {
public:
    CameraMover(GameObject &parent)
        : Component(parent, "CameraMover") {
        // Initialize yaw/pitch from current transform
        glm::vec3 fwd = GetTransform().GetForward();
        yaw = atan2(fwd.z, fwd.x);
        pitch = asin(glm::clamp(fwd.y, -1.0f, 1.0f));
    }
    
    float moveSpeed = 5.0f;
    float lookSpeed = 2.5f;
    
    void Update() override {
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
    
private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float GetDeltaTime() {
        return DeltaTime::GetInstance().GetDeltaTime();
    }
};

#endif // CAMERA_MOVER_H