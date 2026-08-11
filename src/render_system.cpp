#include "render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>

namespace te
{

    struct PushConstantData
    {
        glm::mat2 tranform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    RenderSystem::RenderSystem(TeDevice &device, VkRenderPass renderPass) : teDevice{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    RenderSystem::~RenderSystem()
    {
        vkDestroyPipelineLayout(teDevice.device(), pipelineLayout, nullptr);
    }

    void RenderSystem::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(teDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void RenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        PipelineConfigInfo pipelineConfig{};

        TePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        tePipeline = std::make_unique<TePipeline>(teDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void RenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<TeGameObject> &gameObjects)
    {
        tePipeline->bind(commandBuffer);
        for (auto &obj : gameObjects)
        {
            obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01f, glm::two_pi<float>());

            PushConstantData push{};
            push.offset = obj.transform2D.translation;
            push.color = obj.color;
            push.tranform = obj.transform2D.mat2();
            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}
