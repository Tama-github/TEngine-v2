#pragma once

#include "te_camera.hpp"
#include "te_device.hpp"
#include "te_game_object.hpp"
#include "te_pipeline.hpp"

#include <memory>
#include <vector>

namespace te
{
    class RenderSystem
    {
    public:
        RenderSystem(TeDevice &device, VkRenderPass renderPass);
        ~RenderSystem();
        RenderSystem(const RenderSystem &) = delete;
        RenderSystem &operator=(const RenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<TeGameObject> &gameObjects, const TeCamera &camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderpass);

        TeDevice &teDevice;
        std::unique_ptr<TePipeline> tePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
