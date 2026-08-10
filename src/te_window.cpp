#include "te_window.hpp"

namespace te
{
    TeWindow::TeWindow(int width, int height, std::string name) : width{width}, height{height}, windowName{name}
    {
        initWindow();
    }

    TeWindow::~TeWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void TeWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void TeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface.");
        }
    }

    void TeWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto teWindow = reinterpret_cast<TeWindow *>(glfwGetWindowUserPointer(window));
        teWindow->frameBufferResized = true;
        teWindow->width = width;
        teWindow->height = height;
    }
}
