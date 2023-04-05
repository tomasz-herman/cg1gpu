#version 430 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba8, binding = 0) uniform image2D imgInput;
layout(rgba8, binding = 1) uniform image2D imgOutput;

uniform mat3 kernel;
uniform float divisor;
uniform int imgSize;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    vec4 color = vec4(0, 0, 0, 1);
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            color.rgb += imageLoad(imgInput, clamp(coords + ivec2(i, j), 0, imgSize - 1)).rgb * kernel[i + 1][j + 1];
        }
    }
    color.rgb /= divisor;

    imageStore(imgOutput, coords, color);
}

