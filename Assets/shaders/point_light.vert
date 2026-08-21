#version 450

const vec2 OFFSETS[6] = vec2[](
        vec2(-1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, -1.0),
        vec2(1.0, -1.0),
        vec2(-1.0, 1.0),
        vec2(1.0, 1.0)
    );

layout(location = 0) out vec2 fragOffset;

// linked to GLobalUbo in te_app
layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambiantLightColor; // w is light intensity
    vec4 lightPosition; // w not used, it's for memory alignement
    vec4 lightColor; // w is light intensity
} ubo;

const float LIGHT_RADIUS = 0.1;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {
            ubo.view[0][0],
            ubo.view[1][0],
            ubo.view[2][0]
        };
    vec3 cameraUpWorld = {
            ubo.view[0][1],
            ubo.view[1][1],
            ubo.view[2][1]
        };

    vec3 worldPos = ubo.lightPosition.xyz + LIGHT_RADIUS * fragOffset.x * cameraRightWorld + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;
    gl_Position = ubo.projection * ubo.view * vec4(worldPos, 1.0);
}
