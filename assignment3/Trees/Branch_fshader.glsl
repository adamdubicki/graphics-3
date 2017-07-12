#version 330 core
out vec3 color;

in vec2 uv;
uniform sampler2D tex;
const vec3 COLORS[2] = vec3[](
    vec3(1.0,0.0,0.0),
    vec3(0.0,1.0,0.0));

void main() {
    color = vec3(116.0/255.0, 80.0/255.0, 68.0/255.0);
    // color = COLORS[gl_PrimitiveID];
}


