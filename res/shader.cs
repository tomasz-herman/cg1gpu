#version 430 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba8, binding = 0) uniform image2D imgInput;
layout(rgba8, binding = 1) uniform image2D imgOutput;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    vec4 color = imageLoad(imgInput, coords);
    color.rgb = vec3(1) - color.rgb;

    imageStore(imgOutput, coords, color);
}

