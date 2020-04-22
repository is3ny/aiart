#version 330 core

in vec3 tint;
in vec2 pos;

out vec4 frag_color;

uniform sampler2D canvas;

void main()
{
    vec4 inverted_tx = vec4(vec3(1) - texture(canvas, pos).rgb, 1);
    frag_color = mix(inverted_tx, vec4(tint, 1), 0.2);
    //frag_color = vec4(pos.rg, 0, 1);
}
