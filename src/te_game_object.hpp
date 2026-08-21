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
        TeGameObject(const TeGameObject &) = delete;
        TeGameObject &operator=(const TeGameObject &) = delete;
        TeGameObject(TeGameObject &&) = default;
        TeGameObject &operator=(TeGameObject &&) = default;

        id_t getId() { return id; };

        std::shared_ptr<TeModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        TeGameObject(id_t objId) : id{objId} {};
        id_t id;
    };

}