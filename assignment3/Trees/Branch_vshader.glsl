#version 330 core
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;
uniform float time;
uniform float y_disp;
uniform float scale;

uniform float z_rot;
uniform float y_rot;
uniform float x_rot;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

mat4 S(float s){
    return mat4(mat3(s));
}

mat4 SV(float s){
    return mat4(vec4(1,0,0,0), vec4(0,s,0,0), vec4(0,0,1,0), vec4(0,0,0,1));
}

// col - row
mat4 RZ(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[0][0] =  cos(alpha);
    R[0][1] =  sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] =  cos(alpha);
    return mat4(R);
}

// col - row
mat4 RX(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[1][1] =  cos(alpha);
    R[1][2] =  sin(alpha);
    R[2][1] = -sin(alpha);
    R[2][2] =  cos(alpha);
    return mat4(R);
}

// col - row
mat4 RY(float degrees){
    mat3 R = mat3(1);
    float alpha = radians(degrees);
    R[0][0] =  cos(alpha);
    R[2][0] =  sin(alpha);
    R[0][2] = -sin(alpha);
    R[2][2] =  cos(alpha);
    return mat4(R);
}


void main() {
    // gl_Position = S(.5) * R(10) * vec4(vpoint, 1.0);
    vec4 world = (MODEL * vec4(vpoint + vec3(0, y_disp, 0), 1.0)) * SV(3) * S(.1) * S(scale) * RX(x_rot) * RY(y_rot) * RZ(z_rot);
    gl_Position = PROJ * VIEW * world;
}
