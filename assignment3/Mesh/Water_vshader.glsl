#version 330 core
in vec3 vpoint;
in vec3 vnormal;

out vec2 uv;
out vec3 light;
out vec3 cam;
out float hu;
out float hv;
out float hc;
out float blend;


///--- Uploaded by Eigen in C++
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;
uniform vec3 LIGHT;
uniform vec3 CAM;
uniform float time;
uniform float BLEND;

uniform sampler2D height_map;


void main() {
    float height_offset = 0.2;

    uv = (vpoint.xz + vec2(1.0f,1.0f)) / 2.0f;
    float sum = (vpoint.x * vpoint.x) + (vpoint.z + vpoint.z);
    float displacement = ((sin(sum * time)) * 0.002)/0.5;
    hc = displacement;

    sum = ((vpoint.x + 1) * (vpoint.x + 1)) + (vpoint.z + vpoint.z);
    hu = ((sin(sum * time)) * 0.002)/0.5 + height_offset;

    sum = ((vpoint.x) * (vpoint.x)) + ((vpoint.z + 1)+ (vpoint.z + 1));
    hv = ((sin(sum * time)) * 0.002)/0.5 + height_offset;

    vec4 world = MODEL * vec4(vpoint + vec3(0, displacement+height_offset, 0), 1.0);
    light = LIGHT;
    cam = CAM;
    blend = BLEND;
    gl_Position = PROJ * VIEW * world;
}
