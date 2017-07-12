#version 330 core
out vec3 color;

in vec2 uv;
in vec3 light;
uniform sampler2D tex;
const vec3 COLORS[2] = vec3[](
    vec3(1.0,0.0,0.0),
    vec3(0.0,1.0,0.0));
vec3 SUNRISE = vec3(253.0/255.0,125/255.0, 1/255.0);
vec3 MIDDAY = vec3(113.0/255.0, 215/255.0, 235.0/255.0);
vec3 SUNSET = vec3(254.0/255.0, 91/255.0, 53/255.0);
vec3 NIGHT = vec3(0.1, 0.1, 0.1);

vec3 interpolate(float alpha1, float alpha2, vec3 color1, vec3 color2){
    return (alpha1 * color1) + (alpha2 * color2);
}
vec3 get_color(vec3 light){
    if(light.x > 0 && light.y > 0){
        return interpolate(abs(light.x), abs(light.y), MIDDAY,  MIDDAY);
    } else if (light.x <= 0 && light.y > 0){
        return interpolate(abs(light.y), abs(light.x), MIDDAY,  MIDDAY);
    } else if(light.x < 0 && light.y <= 0){
        return interpolate(abs(light.x), abs(light.y), MIDDAY,  NIGHT);
    } else {
       return interpolate(abs(light.y), abs(light.x), NIGHT,  MIDDAY);
    }

}

void main() {
    vec3 light_vec = normalize(light);
    color = texture(tex,uv).rgb;
    // color = COLORS[gl_PrimitiveID];
}


