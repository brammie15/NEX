#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class CameraComponent : public Component {
public:

    CameraComponent(GameObject &parent);

    float fov = 45.0f;
    float nearClip = 0.1f;
    float farClip  = 100.0f;
    float aspect   = 16.0f / 9.0f;

    glm::mat4 GetViewMatrix() {
        glm::vec3 position = this->GetTransform().GetWorldPosition();
        glm::vec3 forward  = this->GetTransform().GetForward();
        glm::vec3 up       = this->GetTransform().GetUp();
        
        auto lookat = glm::lookAt(position, position + forward, up);
        return lookat;
    }

    glm::mat4 GetProjectionMatrix() {
        return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    }

    void Update() override {
        // printf("Camera Position: (%.2f, %.2f, %.2f)\n",
        //        this->GetTransform().GetWorldPosition().x,
        //        this->GetTransform().GetWorldPosition().y,
        //        this->GetTransform().GetWorldPosition().z);
        auto position = this->GetTransform().GetWorldPosition();
        auto rotation = this->GetTransform().GetWorldRotation();
        //
        printf("Camera position: %f, %f, %f\n", position.x, position.y, position.z);
        printf("Camera Rotation: %f, %f, %f\n", rotation.x, rotation.y, rotation.z);
    }
};

#endif // CAMERA_COMPONENT_H
