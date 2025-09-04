#include "GameObject.h"
#include <string>

#include <iostream>

// #include "Managers/ResourceManager.h"

GameObject::~GameObject() {
    std::cout << "gameobject destroyed: " << GetName() << std::endl;
}

void GameObject::SetActiveDirty() {
    m_ActiveDirty = true;
    for (const Transform* child : m_TransformPtr.GetChildren()) {
        child->GetOwner()->SetActiveDirty();
    }
}

void GameObject::UpdateActiveState() {
    const auto* parentPtr = m_TransformPtr.GetParent();

    if(parentPtr == nullptr) {
        m_ActiveInHierarchy = m_Active;
    } else {
        m_ActiveInHierarchy = m_Active && parentPtr->GetOwner()->IsActiveInHierarchy();
    }

    m_ActiveDirty = false;
}

bool GameObject::IsActiveInHierarchy() {
    if (m_ActiveDirty) {
        UpdateActiveState();
    }

    return m_ActiveInHierarchy;
}

GameObject::GameObject(const std::string& name): Object(name) {
}

void GameObject::Update() {
    for (const auto& component: m_Components) {
        if (!component->HasStarted) {
            component->Start();
            component->HasStarted = true;
        }
        component->Update();
    }
}

void GameObject::LateUpdate() {
    for (const auto& component: m_Components) {
        component->LateUpdate();
    }
}

void GameObject::FixedUpdate() {
    for (const auto& component: m_Components) {
        component->FixedUpdate();
    }
}

void GameObject::Render() const {
    for (const auto& component: m_Components) {
        component->Render();
    }
}

void GameObject::ImGuiRender() {
    for (const auto& component: m_Components) {
        component->ImGuiRender();
    }
}

void GameObject::Destroy() {
    Object::Destroy();

    m_TransformPtr.SetParent(nullptr);

    for (const auto& component: m_Components) {
        component->Destroy();
    }

    for (const auto child : m_TransformPtr.GetChildren()) {
        child->GetOwner()->Destroy();
    }
}

void GameObject::CleanupComponents() {
    std::erase_if(m_Components, [](const std::unique_ptr<Component>& component) {
        return component->IsBeingDestroyed();
    });
}