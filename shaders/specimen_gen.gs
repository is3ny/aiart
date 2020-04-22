// Generate circle primitives given a single point
#version 330 core
#define PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip, max_vertices = 200) out;

uniform int circle_detail;

in VS_OUT {
    vec3 tint;
    float radius;
} gs_in[];

out vec3 tint;
out vec2 pos;

void main()
{
    float radius = gs_in[0].radius;
    
    vec4 origin = gl_in[0].gl_Position;
    vec4 previous = origin + vec4(radius, 0, 0, 0);

    vec2 tex_origin = 0.5 * (origin.xy + vec2(1));
    vec2 tex_previous = 0.5 * (previous.xy + vec2(1));

    tint = gs_in[0].tint;

    for (int i = 1; i <= circle_detail; ++i) {
        float angle = 2 * PI * i / circle_detail;
        vec2 shift = radius * vec2(cos(angle), sin(angle));

        vec4 current = origin + vec4(shift, 0, 0);
        vec2 tex_current = 0.5 * (current.xy + vec2(1));

        gl_Position = origin;
        pos = tex_origin;
        pos = vec2(0, 1);
        EmitVertex();

        gl_Position = previous;
        pos = tex_previous;
        pos = vec2(1.0 * (i - 1) / circle_detail, 0);
        EmitVertex();

        gl_Position = current;
        pos = tex_current;
        pos = vec2(1.0 * i / circle_detail, 0);
        EmitVertex();

        EndPrimitive();

        previous = current;
        tex_previous = tex_current;
    }
}