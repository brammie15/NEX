#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject;

class Transform final {
public:

    explicit Transform(GameObject* owner);
    ~Transform();
    Transform(const Transform&) = delete;
    Transform(Transform&&) noexcept = delete;
    Transform& operator=(const Transform&) = delete;
    Transform& operator=(Transform&&) noexcept = delete;

    [[nodiscard]] const glm::vec3& GetWorldPosition();
    [[nodiscard]] const glm::quat& GetWorldRotation();
    [[nodiscard]] const glm::vec3& GetWorldScale();
    [[nodiscard]] const glm::mat4& GetWorldMatrix();

    [[nodiscard]] glm::vec3 GetForward() const {
        // In OpenGL, forward is -Z in local space
        return glm::normalize(m_WorldRotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    [[nodiscard]] glm::vec3 GetUp() const {
        return glm::normalize(m_WorldRotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    [[nodiscard]] glm::vec3 GetRight() const {
        return glm::normalize(m_WorldRotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }


    void Move(const glm::vec3& move);
    void Move(double x, double y, double z);

    void Rotate(const glm::vec3& axis, float angleRadians);
    void Rotate(const glm::vec3& eulerAngles);
    void Rotate(const glm::quat& delta);


    [[nodiscard]] const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
    [[nodiscard]] const glm::quat& GetLocalRotation() const { return m_LocalRotation; }
    [[nodiscard]] const glm::vec3& GetLocalScale() const { return m_LocalScale; }

    [[nodiscard]] glm::vec3 GetLocalRotationEuler() const {
        return glm::degrees(glm::eulerAngles(m_LocalRotation));
    }

    [[nodiscard]] glm::vec3 GetWorldRotationEuler() const {
        return glm::degrees(glm::eulerAngles(m_WorldRotation));
    }

    void SetLocalRotationEuler(const glm::vec3& eulerAngles) {
        m_LocalRotation = glm::quat(glm::radians(eulerAngles));
        SetRotationDirty();
    }

    void SetWorldRotationEuler(const glm::vec3& eulerAngles) {
        SetWorldRotation(glm::quat(glm::radians(eulerAngles)));
    }

    void SetLocalRotationEuler(float x, float y, float z) {
        SetLocalRotationEuler(glm::vec3{x, y, z});
    }


    void SetLocalPosition(const glm::vec3& position);
    void SetLocalPosition(float x, float y, float z);

    void SetLocalRotation(float x, float y, float z);
    void SetLocalRotation(const glm::vec3& rotation);
    void SetLocalRotation(const glm::quat& rotation);

    void SetLocalScale(float x, float y, float z);
    void SetLocalScale(const glm::vec3& scale);

    void SetWorldPosition(const glm::vec3& position);
    void SetWorldPosition(float x, float y, float z);
    void SetWorldRotation(const glm::quat& rotation);
    void SetWorldRotation(const glm::vec3& rotation);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldScale(double x, double y, double z);
    void SetWorldScale(const glm::vec3& scale);

    // Sets the up vector, keeps the forward vector the same
    void SetUp(const glm::vec3& up) {
        glm::vec3 f = GetForward();
        glm::vec3 u = glm::normalize(up);
        glm::vec3 r = glm::normalize(glm::cross(u, f));
        u = glm::cross(f, r); // make sure up is perpendicular to forward

        glm::mat3 rotMat(r, u, -f); // right, up, -forward
        SetLocalRotation(glm::quat_cast(rotMat));
    }


    glm::vec3 GetForward() {
        return GetWorldRotation() * glm::vec3(0, 0, 1); // or -1 depending on your coordinate system
    }

    glm::vec3 GetRight() {
        return GetWorldRotation() * glm::vec3(1, 0, 0);
    }

    glm::vec3 GetUp() {
        return GetWorldRotation() * glm::vec3(0, 1, 0);
    }

    void SetForward(const glm::vec3& forward) {
        glm::vec3 normalizedForward = glm::normalize(forward);
        glm::vec3 right = glm::normalize(glm::cross(normalizedForward, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::cross(right, normalizedForward);
        
        glm::mat3 rotMatrix = glm::mat3(right, up, -normalizedForward); // Note: might need to adjust signs
        glm::quat rotation = glm::quat_cast(rotMatrix);
        SetWorldRotation(rotation);
    }


    [[nodiscard]] Transform* GetParent() const { return m_Parent; }


    void SetParent(Transform* parent, bool useWorldPosition = true);

    [[nodiscard]] bool IsChild(Transform* child) const;

    [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_Children.size()); }
    [[nodiscard]] const std::vector<Transform*>& GetChildren() const;
    [[nodiscard]] GameObject* GetOwner() const;

private:
    void AddChild(Transform* transform);
    void RemoveChild(Transform* transform);


    void UpdateWorldPosition();
    void UpdateWorldRotation();
    void UpdateWorldScale();
    void UpdateWorldMatrix();

    void SetPositionDirty();
    void SetRotationDirty();
    void SetScaleDirty();

    glm::vec3 m_LocalPosition{};
    glm::quat m_LocalRotation{ glm::mat4{ 1.0f }};
    glm::vec3 m_LocalScale{ 1, 1, 1 };

    bool m_PositionDirty{true};
    bool m_RotationDirty{true};
    bool m_ScaleDirty{true};
    bool m_MatrixDirty{true};

    glm::vec3 m_WorldPosition{};
    glm::quat m_WorldRotation{ glm::mat4{ 1.0f }};
    glm::vec3 m_WorldScale{ 1, 1, 1 };

    glm::mat4 m_WorldMatrix{};


    Transform* m_Parent{};
    std::vector<Transform*> m_Children{};

    GameObject* m_Owner{};
};