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
    
    void Update() override;
    
private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float GetDeltaTime() {
        return DeltaTime::GetInstance().GetDeltaTime();
    }
};

#endif // CAMERA_MOVER_H