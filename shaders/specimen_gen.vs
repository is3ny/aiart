#version 330 core

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 tint;

out VS_OUT {
    vec3 tint;
    float radius;
} vs_out;

void main() 
{
    gl_Position = vec4(vert.xy, 0, 1);
    vs_out.radius = vert.z;
    vs_out.tint = tint;
}