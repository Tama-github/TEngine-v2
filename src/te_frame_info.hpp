#pragma once

#include "te_camera.hpp"

#include <vulkan/vulkan.h>

namespace te
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        TeCamera &camera;
        VkDescriptorSet globalDescriptoSet;
    };
}