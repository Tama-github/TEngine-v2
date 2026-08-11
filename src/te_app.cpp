#include "te_app.hpp"
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

    TeApp::TeApp()
    {
        loadGameObjects();
    }

    TeApp::~TeApp() {}

    void TeApp::run()
    {
        RenderSystem renderSystem{teDevice, teRenderer.getSwapChainRenderPass()};

        while (!teWindow.shouldClose())
        {
            glfwPollEvents();
            if (auto commandBuffer = teRenderer.beginFrame())
            {
                teRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, gameObjects);
                teRenderer.endSwapChainRenderPass(commandBuffer);
                teRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(teDevice.device());
    }

    void TeApp::loadGameObjects()
    {
        std::vector<TeModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto teModel = std::make_shared<TeModel>(teDevice, vertices);
        auto triangle = TeGameObject::createGameObject();
        triangle.model = teModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2D.translation.x = .2f;
        triangle.transform2D.scale = {2.0f, .5f};
        triangle.transform2D.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
}
