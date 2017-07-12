#include "icg_common.h"
#include "imshow.h"

#include <math.h>
#include <OpenGP/GL/Eigen.h>

#include "Mesh/mesh.h"
#include "Trees/tree.h"
#include "Quad/Quad.h"
#include "Quad/CubeMap.h"
#include "catmullrom.h"


using namespace OpenGP;


Mesh grid;
Mesh water;
Quad quadzy1;
Quad quadzy2;
Quad quadxy1;
Quad quadxy2;
Quad quadxz;
Animation_Path p;

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
int window_width = 1280;
int window_height = 1024;
float lastX = 512, lastY = 640;
vec3 cam_pos(-1.7, 2.0, -1.7);
vec3 cam_look(0.0f, 0.0f, 0.0f);
vec3 cam_up(0.0f, 1.0f, 0.0f);
float delta_time = 0.0f;
float last_frame = 0.0f;
bool first_mouse = true;

///- CHANGE THIS FOR MANUAL CONTROL OF CAMERA
bool MANUAL_CAMERA = false;
/// - - - - - - - - - - - - -


void display() {
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    float ratio = window_width / (float) window_height;
    Transform modelTransform = Transform::Identity();
    mat4 model = modelTransform.matrix();
    mat4 projection = OpenGP::perspective(45.0f, ratio, 0.1f, 10.0f);

    //camera movement
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    mat4 view = OpenGP::lookAt(cam_pos, cam_look, cam_up);
    vec3 light = vec3(cos(current_frame), sin(current_frame),0);
//    vec3 light = vec3(1,3,0);

    if(!MANUAL_CAMERA){
        cam_look = p.get_look(current_frame);
        cam_pos = p.get_pos(current_frame);
    }

    grid.draw(model, view, projection, light, cam_pos, false);
    quadxy1.draw(model, view, projection, light);
    quadxy2.draw(model, view, projection, light);
    quadzy1.draw(model, view, projection, light);
    quadzy2.draw(model, view, projection, light);
    quadxz.draw(model, view, projection, light);
    water.draw(model, view, projection, light, cam_pos, true);

}

// https://learnopengl.com/#!Getting-started/Camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    vec3 gaze = cam_look - cam_pos;
    vec3 side = gaze.cross(cam_up);
    gaze.normalize();
    side.normalize();

    if(first_mouse){
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }

    float sensitivity = .0010f;
    xoffset *=sensitivity;
    yoffset *= sensitivity;

    cam_look += (xoffset * side);
    cam_look += (yoffset * cam_up);
}

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    vec3 gaze = cam_look - cam_pos;
    vec3 side = gaze.cross(cam_up);
    gaze.normalize();
    side.normalize();
    float camera_speed = 3.5 * delta_time;

    switch(key){
        case 87:
            cam_pos += camera_speed * gaze;
            cam_look += camera_speed * gaze;
            break;
        case 83:
            cam_pos -= camera_speed * gaze;
            cam_look -= camera_speed * gaze;
            break;
        case 68:
            cam_pos += camera_speed * side;
            cam_look += camera_speed * side;
            break;
        case 65:
            cam_pos -= camera_speed * side;
            cam_look -= camera_speed * side;
            break;
        default:
            break;
    }
}

///
/// You can use this sub-project as a  starting point for your second
/// assignemnt. See the files triangle.h and quad.h for examples of
/// basic OpenGL code.
///

int main(int, char**) {

    OpenGP::glfwInitWindowSize(window_width, window_height);
    OpenGP::glfwMakeWindow("Assignment 3");

    if(MANUAL_CAMERA){
        glfwSetKeyCallback(OpenGP::window, keyboard);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    p.init();
    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    grid.init("Mesh/Mesh_vshader.glsl", "Mesh/Mesh_fshader.glsl");
    water.init("Mesh/Water_vshader.glsl", "Mesh/Water_fshader.glsl");
    GLfloat vp1[] = { /*V1*/ -1.0f, -1.0f, 1.0f,
                   /*V2*/ +1.0f, -1.0f, 1.0f,
                   /*V3*/ -1.0f, +1.0f, 1.0f,
                   /*V4*/ +1.0f, +1.0f, 1.0f };
    quadxy1.init(vp1);
    GLfloat vp2[] = { /*V1*/ -1.0f, -1.0f, -1.0f,
                   /*V2*/ +1.0f, -1.0f, -1.0f,
                   /*V3*/ -1.0f, +1.0f, -1.0f,
                   /*V4*/ +1.0f, +1.0f, -1.0f };
    quadxy2.init(vp2);
    GLfloat vp3[] = { /*V1*/ 1.0f, -1.0f, +1.0f,
                   /*V2*/ 1.0f, -1.0f, -1.0f,
                   /*V3*/ 1.0f, +1.0f, +1.0f,
                   /*V4*/ 1.0f, +1.0f, -1.0f };
    quadzy1.init(vp3);
    GLfloat vp4[] = { /*V1*/ -1.0f, -1.0f, +1.0f,
                   /*V2*/ -1.0f, -1.0f, -1.0f,
                   /*V3*/ -1.0f, +1.0f, +1.0f,
                   /*V4*/ -1.0f, +1.0f, -1.0f };
    quadzy2.init(vp4);
    GLfloat vp5[] = { /*V1*/ -1.0f, 1.0f, -1.0f,
                   /*V2*/ +1.0f, 1.0f, -1.0f,
                   /*V3*/ -1.0f, 1.0f, +1.0f,
                   /*V4*/ +1.0f, 1.0f, +1.0f };
    quadxz.init(vp5);


    OpenGP::glfwDisplayFunc(&display);
    OpenGP::glfwMainLoop();

    return EXIT_SUCCESS;

}
