#include "config.h"
#include "te_app.hpp"

int main()
{
    try
    {
        te::TeApp app{};
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
