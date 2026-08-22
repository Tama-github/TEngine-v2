#include "te_app.hpp"

#include "kb_movement_controller.hpp"
#include "te_buffer.hpp"
#include "te_camera.hpp"
#include "systems/render_system.hpp"
#include "systems/point_light_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace te
{

    TeApp::TeApp()
    {
        globalPool = TeDescriptorPool::Builder(teDevice)
                         .setMaxSets(TeSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TeSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .build();
        loadGameObjects();
    }

    TeApp::~TeApp() {}

    void TeApp::run()
    {
        std::vector<std::unique_ptr<TeBuffer>> uboBuffers(TeSwapChain::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<TeBuffer>(
                teDevice,
                sizeof(GlobalUbo),
                TeSwapChain::MAX_FRAMES_IN_FLIGHT,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT /*| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT*/); // this flag is to avoid to flush the memory
            uboBuffers[i]->map();
        }

        auto globalSetLayout =
            TeDescriptorSetLayout::Builder(teDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(TeSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            TeDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        RenderSystem renderSystem{
            teDevice,
            teRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{
            teDevice,
            teRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};
        TeCamera camera{};

        auto viewerObject = TeGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KBMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!teWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlanXZ(teWindow.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = teRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
            if (auto commandBuffer = teRenderer.beginFrame())
            {
                int frameIndex = teRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects,
                };

                // update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                teRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
                teRenderer.endSwapChainRenderPass(commandBuffer);
                teRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(teDevice.device());
    }

    void TeApp::loadGameObjects()
    {
        std::shared_ptr<TeModel> teModel = TeModel::createModelFromFile(teDevice, "Assets/models/smooth_vase.obj");
        auto obj = TeGameObject::createGameObject();
        obj.model = teModel;
        obj.transform.translation = {0.0f, 0.5f, 0.0f};
        obj.transform.scale = glm::vec3(3.0f);
        gameObjects.emplace(obj.getId(), std::move(obj));

        teModel = TeModel::createModelFromFile(teDevice, "Assets/models/quad.obj");
        auto quad = TeGameObject::createGameObject();
        quad.model = teModel;
        quad.transform.translation = {0.0f, 0.5f, 0.0f};
        quad.transform.scale = glm::vec3(3.0f);
        gameObjects.emplace(quad.getId(), std::move(quad));

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f} //
        };

        for (int i = 0; i < lightColors.size(); i++)
        {
            auto pointLight = TeGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(glm::mat4{1.0f}, i * glm::two_pi<float>() / lightColors.size(), {0.0f, -1.0f, 0.0f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
            // pointLight is not valid after std::move so we can protect its usage by puting it inside of the scope limitation
        }
    }
}
