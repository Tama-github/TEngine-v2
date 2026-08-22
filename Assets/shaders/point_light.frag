#version 450

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

// linked in point_light_system.cpp
struct PointLight {
    vec4 position; // w not used, it's for memory alignement
    vec4 color; // w is light intensity
};

// linked to GLobalUbo in te_frame_info.hpp
layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambiantLightColor; // w is light intensity
    PointLight pointLights[10];
    int numLights;
} ubo;

layout(push_constant) uniform Push {
    vec4 position;
    vec4 color;
    float radius;
} push;

void main() {
    float dist = sqrt(dot(fragOffset, fragOffset));
    if (dist >= 1.0) {
        discard;
    }
    outColor = vec4(push.color.xyz, 1.0);
}
