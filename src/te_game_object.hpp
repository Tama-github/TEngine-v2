#pragma once

#include "te_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace te
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation;
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent
    {
        float lightIntensity = 1.0f;
    };

    class TeGameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, TeGameObject>;

        static TeGameObject createGameObject()
        {
            static id_t currentId = 0;
            return TeGameObject{currentId++};
        }

        static TeGameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f));

        TeGameObject(const TeGameObject &) = delete;
        TeGameObject &operator=(const TeGameObject &) = delete;
        TeGameObject(TeGameObject &&) = default;
        TeGameObject &operator=(TeGameObject &&) = default;

        id_t getId() { return id; };

        glm::vec3 color{};
        TransformComponent transform{};

        // Optional pointer components
        std::shared_ptr<TeModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        TeGameObject(id_t objId) : id{objId} {};
        id_t id;
    };

}