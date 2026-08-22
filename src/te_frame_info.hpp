#pragma once

#include "te_camera.hpp"
#include "te_game_object.hpp"

#include <vulkan/vulkan.h>

#define MAX_LIGHTS 10

namespace te
{

    struct PointLight
    {
        glm::vec4 position{}; // w not used, it's for memory alignement
        glm::vec4 color{};    // w is light intensity
    };

    // linked as [layout(set = 0, binding = 0) uniform GlobalUbo ...] inside the shaders
    struct GlobalUbo
    {
        glm::mat4 projection{1.0f};
        glm::mat4 view{1.0f};
        glm::vec4 ambiantLightColor{1.0, 1.0, 1.0, 0.02}; // w is light intensity
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        TeCamera &camera;
        VkDescriptorSet globalDescriptoSet;
        TeGameObject::Map &gameObjects;
    };
}