#pragma once

#include "te_descriptors.hpp"
#include "te_device.hpp"
#include "te_game_object.hpp"
#include "te_window.hpp"
#include "te_renderer.hpp"

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
        TeApp &operator=(const TeApp &) = delete;

        void run();

    private:
        void loadGameObjects();

        TeWindow teWindow{WINDOW_WIDTH, WINDOW_HEIGHT, "TEngine"};
        TeDevice teDevice{teWindow};
        TeRenderer teRenderer{teWindow, teDevice};

        // note: order of declaration matters
        std::unique_ptr<TeDescriptorPool> globalPool{};
        std::vector<TeGameObject> gameObjects;
    };
}
