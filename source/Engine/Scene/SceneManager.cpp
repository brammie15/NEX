#include "SceneManager.h"

#include <stdexcept>

#include "Scene.h"
// #include "Input/InputManager.h"

void SceneManager::Update() {
    m_scenes[m_ActiveSceneIndex]->Update();
}

void SceneManager::FixedUpdate() {
    m_scenes[m_ActiveSceneIndex]->FixedUpdate();
}

void SceneManager::LateUpdate() {
    m_scenes[m_ActiveSceneIndex]->LateUpdate();
}

void SceneManager::Render() {
    m_scenes[m_ActiveSceneIndex]->Render();
}

void SceneManager::RenderImgui() {
    m_scenes[m_ActiveSceneIndex]->RenderImgui();
}

void SceneManager::HandleGameObjectDestroy() {
    for (const auto& scene : m_scenes) {
        scene->CleanupDestroyedGameObjects();
    }
}

void SceneManager::DestroyGameObjects() {
    for (const auto& scene: m_scenes) {
        scene->DestroyGameObjects();
    }
}

void SceneManager::UnloadAllScenes() {
    for (const auto& scene : m_scenes) {
        scene->Unload();
    }
}

void SceneManager::HandleSceneDestroy() {
    for (auto it = m_scenes.begin(); it != m_scenes.end();) {
        if ((*it)->IsBeingUnloaded()) {
            it = m_scenes.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneManager::HandleScene() {
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

void SceneManager::Destroy() {
    UnloadAllScenes();
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

void SceneManager::SwitchScene(int index) {
    // InputManager::GetInstance().RemoveAllBindings();

    if (index < 0 || index >= static_cast<int>(m_scenes.size())) {
        // throw std::out_of_range("Scene index out of range");
    }
    m_scenes[m_ActiveSceneIndex]->UnloadBindings();
    m_ActiveSceneIndex = index;
    m_scenes[m_ActiveSceneIndex]->LoadBindings();
}

Scene &SceneManager::CreateScene(const std::string &name) {
    const auto &scene = std::shared_ptr<Scene>(new Scene(name));
    m_scenes.push_back(scene);
    return *scene;
}