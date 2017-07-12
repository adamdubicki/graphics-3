#version 330 core
out vec3 color;

in vec2 uv;
in vec3 light;
in vec3 cam;

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
vec3 get_color(vec3 light){
    if(light.x > 0 && light.y > 0){
        return interpolate(abs(light.x), abs(light.y), SUNRISE,  MIDDAY);
    } else if (light.x <= 0 && light.y > 0){
        return interpolate(abs(light.y), abs(light.x), MIDDAY,  SUNSET);
    } else if(light.x < 0 && light.y <= 0){
        return interpolate(abs(light.x), abs(light.y), SUNSET,  NIGHT);
    } else {
       return interpolate(abs(light.y), abs(light.x), NIGHT,  SUNSET);
    }
}

void main() {

    float tiling_amount = 5;

    float height_center = texture(height_map, uv).r;

    float height_plus_u = textureOffset(height_map, uv, ivec2(0,1)).r;
    float height_plus_v = textureOffset(height_map, uv, ivec2(1,0)).r;

    vec3 a = vec3(1, 500 * (height_plus_v - height_center) , 0);
    vec3 b = vec3(0, 500 * (height_plus_u - height_center), 1);

    vec3 N = normalize(cross(b,a));
    vec3 reflection = reflect(normalize(vec3(uv.x, height_center, uv.y) - light) ,N);
    vec3 V = cam - vec3(uv.x, height_center, uv.y);
    V = normalize(V);
    vec3 offset_ambience = get_color(light);

    vec3 light = normalize(light);
    vec3 ambient = vec3(0.1, 0.1, 0.1) + offset_ambience * 0.1;
    vec3 diffuse = texture(diffuse_map, uv * tiling_amount).rgb * clamp(dot(N, light), 0, 1);
    float spec = pow(max(dot(V, reflection),0.0),32);
    vec3 specular = spec * 0.1 * vec3(0.2,0.2,0.2);
    color = ambient + diffuse;
    if(height_center > 0.4 && acos(dot(N, vec3(0,1,0))) < 1.3){
        color = vec3(0.85, 0.85, 0.85) * clamp(dot(N, light), 0, 1) + ambient + specular;
     }

}
