#pragma once

#include "../te_camera.hpp"
#include "../te_device.hpp"
#include "../te_game_object.hpp"
#include "../te_pipeline.hpp"
#include "../te_frame_info.hpp"

#include <memory>
#include <vector>

namespace te
{
    class PointLightSystem
    {
    public:
        PointLightSystem(TeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();
        PointLightSystem(const PointLightSystem &) = delete;
        PointLightSystem &operator=(const PointLightSystem &) = delete;

        void update(FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderpass);

        TeDevice &teDevice;
        std::unique_ptr<TePipeline> tePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
