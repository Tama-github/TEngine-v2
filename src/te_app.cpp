#include "te_app.hpp"

#include "kb_movement_controller.hpp"
#include "te_buffer.hpp"
#include "te_camera.hpp"
#include "render_system.hpp"

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

    struct GlobalUbo
    {
        glm::mat4 projectionView{1.0f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0, -3.0, -1.0});
    };

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
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
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
        TeCamera camera{};

        auto viewerObject = TeGameObject::createGameObject();
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
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
            if (auto commandBuffer = teRenderer.beginFrame())
            {
                int frameIndex = teRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                };

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                teRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(frameInfo, gameObjects);
                teRenderer.endSwapChainRenderPass(commandBuffer);
                teRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(teDevice.device());
    }

    void TeApp::loadGameObjects()
    {
        std::shared_ptr<TeModel> teModel = TeModel::createModelFromFile(teDevice, "Assets/models/flat_vase.obj");
        auto obj = TeGameObject::createGameObject();
        obj.model = teModel;
        obj.transform.translation = {0.0f, 0.5f, 2.5f};
        obj.transform.scale = glm::vec3(3.0f);
        gameObjects.push_back(std::move(obj));
    }
}
