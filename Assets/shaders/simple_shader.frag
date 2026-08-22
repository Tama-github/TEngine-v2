#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;

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
    vec4 ambientLightColor; // w is light intensity
    PointLight pointLights[10];
    int numLights;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 surfaceNormal = normalize(fragWorldNormal);

    for (int i = 0; i < ubo.numLights; i++) {
        PointLight light = ubo.pointLights[i];
        vec3 dirToLight = light.position.xyz - fragWorldPos;
        float attenuationFactor = 1.0 / dot(dirToLight, dirToLight); // efficient way to caluculate square vector dist;
        float cosAngIncidence = max(dot(surfaceNormal, normalize(dirToLight)), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuationFactor;

        diffuseLight += intensity * cosAngIncidence;
    }

    outColor = vec4(diffuseLight * fragColor, 1.0);
}
