#pragma once

#include "te_device.hpp"
#include "te_swap_chain.hpp"
#include "te_window.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace te
{
    class TeRenderer
    {
    public:
        TeRenderer(TeWindow &window, TeDevice &device);
        ~TeRenderer();
        TeRenderer(const TeRenderer &) = delete;
        TeRenderer &operator=(const TeRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return teSwapChain->getRenderPass(); };
        float getAspectRatio() const { return teSwapChain->extentAspectRatio(); };
        bool isFrameInProgress() const { return isFrameStarted; };
        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        };

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        };

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffer();
        void freeCommandBuffers();
        void recreateSwapChain();

        TeWindow &teWindow;
        TeDevice &teDevice;
        std::unique_ptr<TeSwapChain> teSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;
    };
}
