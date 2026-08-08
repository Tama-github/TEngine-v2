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
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        GLFWwindow *window;
        const int width;
        const int height;
        std::string windowName;

        void initWindow();
    };

}
