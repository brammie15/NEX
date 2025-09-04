
#include "Component.h"

#include <stdexcept>
#include "GameObject.h"
#include "imgui.h"

Component::Component(GameObject& pParent, const std::string& name): Object(name), m_ParentGameObjectPtr(&pParent) {
    // if (m_ParentGameObjectPtr == nullptr) {
    //     //TODO: Change pParent ot be a reference
    //     throw std::runtime_error("Component made with no GameObject??");
    // }
}

Transform& Component::GetTransform() const {
    return m_ParentGameObjectPtr->GetTransform();
}

void Component::Destroy() {
    // const bool isBeingDestroyed = GetIsBeingDestroyed();
    Object::Destroy();
}

void Component::Start() {
}

void Component::SetEnabled(bool enabled) {
    m_IsEnabled = enabled;
}

void Component::LateUpdate() {
}

void Component::FixedUpdate() {
}

void Component::ImGuiInspector() {
}

void Component::ImGuiRender() {
}

void Component::Render() {

}