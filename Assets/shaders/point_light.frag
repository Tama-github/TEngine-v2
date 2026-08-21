#version 450

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

// linked to GLobalUbo in te_app
layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambiantLightColor; // w is light intensity
    vec4 lightPosition; // w not used, it's for memory alignement
    vec4 lightColor; // w is light intensity
} ubo;

void main() {
    float dist = sqrt(dot(fragOffset, fragOffset));
    if (dist >= 1.0) {
        discard;
    }
    outColor = vec4(ubo.lightColor.xyz, 1.0);
}
