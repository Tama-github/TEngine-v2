cmake_minimum_required(VERSION 4.0.0)
project(TEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(LOCAL_INCLUDE_DIR "C:/lib")

find_package(GLFW3 REQUIRED)
find_package(Vulkan)

add_executable(TEngine
    src/config.h
    src/te_utils.hpp
    src/te_utils.cpp
    src/te_window.hpp
    src/te_window.cpp
    src/te_app.hpp
    src/te_app.cpp
    src/te_pipeline.hpp
    src/te_pipeline.cpp
    src/te_device.hpp
    src/te_device.cpp
    src/te_swap_chain.hpp
    src/te_swap_chain.cpp
    src/te_model.hpp
    src/te_model.cpp
    src/te_game_object.hpp
    src/te_game_object.cpp
    src/te_renderer.hpp
    src/te_renderer.cpp
    src/render_system.hpp
    src/render_system.cpp
    src/te_camera.hpp
    src/te_camera.cpp
    src/kb_movement_controller.hpp
    src/kb_movement_controller.cpp
    src/main.cpp
)

target_include_directories(TEngine PRIVATE dependencies)
target_include_directories(TEngine PRIVATE ${LOCAL_INCLUDE_DIR}/tinyobjectloader)


target_link_libraries(TEngine Vulkan::Vulkan)
target_link_libraries(TEngine ${GLFW3_LIBRARY})
target_link_libraries(TEngine )

add_custom_target(
    compileshaders
    COMMAND cmd /c ${CMAKE_SOURCE_DIR}/compile.bat
    COMMENT "compile shaders..."
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(copyassets
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/Assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/bin/Assets
  COMMENT "copying assets..."
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_dependencies(copyassets compileshaders)
add_dependencies(TEngine copyassets)
