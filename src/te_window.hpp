#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

namespace te
{

    class TeWindow
    {
    public:
        TeWindow(int width, int height, std::string name);
        ~TeWindow();
        TeWindow(const TeWindow &) = delete;
        TeWindow &operator=(const TeWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };
        bool hasWindowResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        GLFWwindow *window;
        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;

        void initWindow();
    };

}
