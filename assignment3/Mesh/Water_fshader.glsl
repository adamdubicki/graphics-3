#version 330 core
out vec4 color;

in vec2 uv;
in vec3 light;
in vec3 cam;
in float hu;
in float hv;
in float hc;
in float blend;

uniform sampler2D height_map;
uniform sampler2D diffuse_map;

vec3 reflect(vec3 v, vec3 n){
    return v - 2 * dot(v,n)*n;
}

vec3 SUNRISE = vec3(253.0/255.0,125/255.0, 1/255.0);
vec3 MIDDAY = vec3(113.0/255.0, 215/255.0, 235.0/255.0);
vec3 SUNSET = vec3(254.0/255.0, 91/255.0, 53/255.0);
vec3 NIGHT = vec3(0.1, 0.1, 0.1);

vec3 interpolate(float alpha1, float alpha2, vec3 color1, vec3 color2){
    return (alpha1 * color1) + (alpha2 * color2);
}


void main() {

    float tiling_amount = 5;

    float height_center = hc;
    float height_plus_u = hu;
    float height_plus_v = hv;

    vec3 a = vec3(1, 1000*(height_plus_v - height_center) , 0);
    vec3 b = vec3(0, 1000*(height_plus_u - height_center), 1);
    vec3 N = normalize(cross(b,a));
    // For specular
    vec3 reflection = reflect(normalize(vec3(uv.x, height_center, uv.y))-light ,N);
    vec3 V = cam - vec3(uv.x, height_center, uv.y);
    V = normalize(V);

    vec3 light = normalize(light);
    vec3 ambient = vec3(0.0,0.4,0.45);
    vec3 diffuse = vec3(0.0,0.4,0.45)* clamp(dot(N, light), 0, 1);
    float spec = pow(5,max(dot(V, reflection),0.0));
    vec3 specular = vec3(0.05,0.05,0.05) * spec * 0.5;
    if(blend == 1){
         color = vec4(ambient + diffuse +specular ,0.3);
    } else {
         color = vec4(ambient + diffuse+specular,1.0);
    }
}
