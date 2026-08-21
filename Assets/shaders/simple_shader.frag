#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;

layout(location = 0) out vec4 outColor;

// linked to GLobalUbo in te_app
layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambiantLightColor; // w is light intensity
    vec4 lightPosition; // w not used, it's for memory alignement
    vec4 lightColor; // w is light intensity
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 dirToLight = ubo.lightPosition.xyz - fragWorldPos;
    float attenuationFactor = 1.0 / dot(dirToLight, dirToLight); // eficient way to caluculate square vector dist;

    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuationFactor;
    vec3 ambiantLight = ubo.ambiantLightColor.xyz * ubo.ambiantLightColor.w;
    vec3 diffuseLight = lightColor * max(dot(normalize(fragWorldNormal), normalize(dirToLight)), 0);

    outColor = vec4((diffuseLight + ambiantLight) * fragColor, 1.0);
}
