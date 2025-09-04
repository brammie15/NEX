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
        // initialize yaw/pitch from current transform forward vector
        glm::vec3 fwd = GetTransform().GetForward();
        yaw   = atan2(fwd.z, fwd.x);                // yaw around Y
        pitch = asin(glm::clamp(fwd.y, -1.0f, 1.0f)); // pitch from Y
    }

    float moveSpeed   = 5.0f;   // units per second
    float lookSpeed   = 2.5f;   // radians per second

    void Update() override {
        auto &t = GetTransform();
        auto &input = InputManager::GetInstance();
        float dt = GetDeltaTime();

        // --- Movement (left stick) ---
        glm::vec2 leftstick = input.GetStick(0);
        glm::vec3 forward = GetForwardVector();               // from yaw/pitch
        glm::vec3 right   = glm::normalize(glm::cross(forward, worldUp));

        glm::vec3 moveDelta = (forward * leftstick.y + right * leftstick.x) * moveSpeed * dt;
        t.SetWorldPosition(t.GetWorldPosition() + moveDelta);

        // --- Look (right stick) ---
        glm::vec2 rightstick = input.GetStick(1);
        yaw   += -rightstick.x * lookSpeed * dt;
        pitch += -rightstick.y * lookSpeed * dt;

        // clamp pitch to avoid flipping
        float limit = glm::radians(89.0f);
        pitch = glm::clamp(pitch, -limit, limit);

        // --- Apply rotation ---
        forward = GetForwardVector();
        t.SetForward(forward); // recalculates rotation quaternion internally
        // SetUp is optional here, because SetForward already rebuilds up from worldUp
    }

private:
    float yaw   = 0.0f;
    float pitch = 0.0f;
    const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float GetDeltaTime() {
        return DeltaTime::GetInstance().GetDeltaTime();
    }

    glm::vec3 GetForwardVector() const {
        glm::vec3 fwd;
        fwd.x = cos(pitch) * cos(yaw);
        fwd.y = sin(pitch);
        fwd.z = cos(pitch) * sin(yaw);
        return glm::normalize(fwd);
    }
};
 
#endif // CAMERA_MOVER_H
