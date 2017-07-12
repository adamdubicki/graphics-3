#include "icg_common.h"

vec3 get_cr_spline(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t){
    return 0.5 * ( (2 * p1) + (-p0 + p2) * t + (2*p0 - 5*p1 + 4*p2 - p3) * pow(t,2) + (-p0 + 3*p1- 3*p2 + p3) * pow(t,3));
}

float get_decimal(float t){
    int base = int(t);
    return t - base;
}

struct spline {
    vec3 p0;
    vec3 p1;
    vec3 p2;
    vec3 p3;
};


class Animation_Path{
public:
    void init(){
        cam_move_path[0] = {
            vec3(-2.85, 1.707f, 2.217),
            vec3(-2.85, 1.707f, 2.217),
            vec3(-2.85, 1.707f, -2.217),
            vec3(-2.85, 1.707f, -2.217)
        };
        cam_move_path[1] = {
            vec3(-2.85, 1.707f, -2.217),
            vec3(-2.85, 1.707f, -2.217),
            vec3(2.85, 1.707f, -2.217),
            vec3(2.85, 1.707f, -2.217)
        };
        cam_move_path[2] = {
            vec3(2.85, 1.707f, -2.217),
            vec3(2.85, 1.707f, -2.217),
            vec3(2.85, 1.707f,  2.217),
            vec3(2.85, 1.707f, 2.217)
        };
        cam_move_path[3] = {
            vec3(2.85, 1.707f, 2.217),
            vec3(2.85, 1.707f, 2.217),
            vec3(-2.85, 1.707f, 2.217),
            vec3(-2.85, 1.707f, 2.217)
        };
        cam_move_path[4] = {
            vec3(-2.85, 1.707f, 2.217),
            vec3(-2.85, 1.707f, 2.217),
            vec3(-0.556002, 0.694853f, 0.646225),
            vec3(-0.556002, 0.694853f, 0.646225)
        };
        cam_move_path[5] = {
            vec3(-0.556002, 0.694853f, 0.646225),
            vec3(-0.556002, 0.694853f, 0.646225),
            vec3(0.089, 0.5368f, -0.0879),
            vec3(0.089, 0.5368f, -0.0879)
        };
        cam_move_path[6] = {
            vec3(0.089, 0.5368f, -0.0879),
            vec3(0.089, 0.5368f, -0.0879),
            vec3(0.75, 2.18f, -0.85),
            vec3(0.75, 2.18f, -0.85)
        };

        cam_look_path[0] = {
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0)
        };
        cam_look_path[1] = {
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0)
        };
        cam_look_path[2] = {
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0)
        };
        cam_look_path[3] = {
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(0,0,0)
        };
        cam_look_path[4] = {
            vec3(0,0,0),
            vec3(0,0,0),
            vec3(1.72,-1.49,-2.943925),
            vec3(1.72,-1.49,-2.943925)
        };
        cam_look_path[5] = {
            vec3(1.72,-1.49,-2.943925),
            vec3(1.72,-1.49,-2.943925),
            vec3(2.38,-1.537,0.84),
            vec3(2.38,-1.537,0.84)
        };
        cam_look_path[6] = {
            vec3(2.38,-1.537,0.84),
            vec3(2.38,-1.537,0.84),
            vec3(-1.13,-0.686,0.85),
            vec3(-1.13,-0.686,0.85)
        };
    }

    vec3 get_look(float time){
        int spline_index = (int(time) % NUM_SPLINES);
        struct spline s = cam_look_path[spline_index];
        return get_cr_spline(s.p0, s.p1, s.p2, s.p3, get_decimal(time));
    }

    vec3 get_pos(float time){
        int spline_index = (int(time) % NUM_SPLINES);
        struct spline s = cam_move_path[spline_index];
        return get_cr_spline(s.p0, s.p1, s.p2, s.p3, get_decimal(time));
    }

    static const int NUM_SPLINES = 7;
    struct spline cam_move_path[NUM_SPLINES];
    struct spline cam_look_path[NUM_SPLINES];
};
