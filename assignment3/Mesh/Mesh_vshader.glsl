#version 330 core 
in vec3 vpoint;
in vec3 vnormal;

out vec2 uv;
out vec3 light;
out vec3 cam;

///--- Uploaded by Eigen in C++
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;
uniform vec3 LIGHT;
uniform vec3 CAM;
uniform vec3 TIME;

uniform sampler2D height_map;


void main() {

     uv = (vpoint.xz + vec2(1.0f,1.0f)) / 2.0f;

    float displacement = texture(height_map, uv).r;
    vec4 world = MODEL * vec4(vpoint + vec3(0, displacement, 0), 1.0);
    light = LIGHT;
    cam = CAM;
    gl_Position = PROJ * VIEW * world;
}
