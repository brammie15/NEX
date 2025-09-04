#include "CameraComponent.h"

CameraComponent::CameraComponent(GameObject& parent): Component(parent, "Camera"),
                                                      fov(45.0f), nearClip(0.1f), farClip(100.0f), aspect(16.0f / 9.0f) {
}