#version 330 core
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;
out vec3 light;
uniform float time;
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;
uniform vec3 LIGHT;

mat4 S(float s){
    return mat4(mat3(s));
}

mat4 R(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[0][0] =  cos(alpha);
    R[0][1] =  sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] =  cos(alpha);
    return mat4(R);
}

void main() {
    vec4 world = MODEL * vec4(vpoint, 1.0) * S(5);
    gl_Position = PROJ * VIEW * world;
    light = LIGHT;
    uv = vtexcoord;
}
