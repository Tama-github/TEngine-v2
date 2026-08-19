#pragma once

#include "te_camera.hpp"
#include "te_device.hpp"
#include "te_game_object.hpp"
#include "te_pipeline.hpp"
#include "te_frame_info.hpp"

#include <memory>
#include <vector>

namespace te
{
    class RenderSystem
    {
    public:
        RenderSystem(TeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();
        RenderSystem(const RenderSystem &) = delete;
        RenderSystem &operator=(const RenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo, std::vector<TeGameObject> &gameObjects);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderpass);

        TeDevice &teDevice;
        std::unique_ptr<TePipeline> tePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
