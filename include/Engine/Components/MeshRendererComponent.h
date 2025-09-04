#include "Component.h"
#include "Transform.h"

#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Shader.h"

class MeshRendererComponent : public Component {
public:
    MeshRendererComponent(GameObject& parent, const std::string& modelPath)
        : Component(parent, "MeshRenderer"),
          m_model(std::make_unique<Model>(modelPath)),
          m_shader(std::make_unique<Shader>()) {
        if (!m_model->load()) {
            printf("Failed to load model: %s\n", modelPath.c_str());
        } else {
            if (!m_model->uploadToGPU()) {
                printf("Failed to upload model to GPU: %s\n", modelPath.c_str());
            }
        }

        std::string vertexShaderPath = "romfs:/shaders/vertex.glsl";
        std::string fragmentShaderPath = "romfs:/shaders/fragment.glsl";

        //Check if file exists
        FILE* file = fopen(vertexShaderPath.c_str(), "r");
        if (file) {
            fclose(file);
        } else {
            printf("Vertex shader file not found: %s\n", vertexShaderPath.c_str());
        }

        if (!m_shader->loadFromFiles(vertexShaderPath, fragmentShaderPath)) {
            printf("Failed to load shader: %s and %s\n", vertexShaderPath.c_str(), fragmentShaderPath.c_str());
        }
    }


    void Update() override {
    }

    Model* GetModel() const { return m_model.get(); }
    Shader* GetShader() const { return m_shader.get(); }
private:
    std::unique_ptr<Model> m_model;
    std::unique_ptr<Shader> m_shader;
};