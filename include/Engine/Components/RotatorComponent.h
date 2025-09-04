#pragma once
#include "Component.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class RotatorComponent : public Component {
public:
    RotatorComponent(GameObject& parent, float radius, float speed)
        : Component(parent, "Rotator"),
          m_radius(radius),
          m_speed(speed),
          m_angle(0.0f) {}

    void Update() override {
        double deltaTime = DeltaTime::GetInstance().GetDeltaTime();
        // printf("deltaTime: %f\n", deltaTime);
        m_angle += m_speed;

        // keep angle in [0, 2π]
        if (m_angle > glm::two_pi<float>()) {
            m_angle -= glm::two_pi<float>();
        }

        // Compute new position in a circle (around world origin or parent pivot)
        glm::vec3 center(0.0f, 0.0f, 0.0f); // orbit center
        glm::vec3 pos(
            center.x + m_radius * cos(m_angle),
            center.y,
            center.z + m_radius * sin(m_angle)
        );

        // Apply to Transform
        GetTransform().SetWorldPosition(pos);
    }

private:
    float m_radius;   // distance from center
    float m_speed;    // radians per frame
    float m_angle;    // current angle
};
