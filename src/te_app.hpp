#pragma once

#include "te_window.hpp"
#include "te_pipeline.hpp"
#include "te_device.hpp"
#include "te_swap_chain.hpp"
#include "te_model.hpp"

#include <memory>
#include <vector>

namespace te
{
    class TeApp
    {
    public:
        static constexpr int WINDOW_WIDTH = 640;
        static constexpr int WINDOW_HEIGHT = 480;

        TeApp();
        ~TeApp();
        TeApp(const TeApp &) = delete;
        TeApp operator=(const TeApp &) = delete;

        void run();

    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffer();
        void drawFrame();

        TeWindow teWindow{WINDOW_WIDTH, WINDOW_HEIGHT, "TEngine"};
        TeDevice teDevice{teWindow};
        TeSwapChain teSwapChain{teDevice, teWindow.getExtent()};
        // TePipeline tePipeline{teDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", TePipeline::defaultPipelineConfigInfo(WINDOW_WIDTH, WINDOW_HEIGHT)};
        std::unique_ptr<TePipeline> tePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<TeModel> teModel;
    };
}
