cmake_minimum_required(VERSION 4.0.0)
project(TEngine VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

find_package(GLFW3 REQUIRED)
find_package(Vulkan)

add_executable(TEngine 
    src/config.h
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
    src/main.cpp
)

target_include_directories(TEngine PRIVATE dependencies)

target_link_libraries(TEngine Vulkan::Vulkan)
target_link_libraries(TEngine ${GLFW3_LIBRARY})

add_custom_target(
    compileshaders
    COMMAND cmd /c ${CMAKE_CURRENT_SOURCE_DIR}/compile.bat
)

add_custom_target(copyshaders
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/bin/shaders
  COMMENT "copying shaders..."
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_dependencies(TEngine copyshaders)
