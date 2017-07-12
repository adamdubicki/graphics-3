#pragma once
#include "icg_common.h"
#include "OpenGP/Eigen/Image.h"
#include "../noise.h"

class Mesh{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID
    GLuint _height_tex;
    GLuint _diffuse_tex;

    const std::string filename = "Mesh/grid.obj";
    OpenGP::SurfaceMesh mesh;
    GLuint _vpoint;    ///< memory buffer
    GLuint _vnormal;   ///< memory buffer

public:
    GLuint getProgramID(){ return _pid; }

    void init(const char* vshader, const char* fshader){
        ///--- Compile the shaders
        _pid = OpenGP::load_shaders(vshader, fshader);
        if(!_pid) exit(EXIT_FAILURE);
        check_error_gl();
        bool success = mesh.read(filename.c_str());
        assert(success);
        mesh.triangulate();
        mesh.update_vertex_normals();
        printf("Loaded mesh '%s' (#V=%d, #F=%d)\n", filename.c_str(), mesh.n_vertices(), mesh.n_faces());

        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        ///--- Vertex Buffer
        OpenGP::SurfaceMesh::Vertex_property<OpenGP::Point> vpoints = mesh.get_vertex_property<OpenGP::Point>("v:point");
        glGenBuffers(ONE, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
        check_error_gl();

        ///--- Normal Buffer
        OpenGP::SurfaceMesh::Vertex_property<OpenGP::Normal> vnormals = mesh.get_vertex_property<OpenGP::Normal>("v:normal");
        glGenBuffers(ONE, &_vnormal);
        glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
        glBufferData(GL_ARRAY_BUFFER, mesh.n_vertices() * sizeof(vec3), vnormals.data(), GL_STATIC_DRAW);
        check_error_gl();

        ///--- Index Buffer
        std::vector<unsigned int> indices;
        for(OpenGP::SurfaceMesh::Face_iterator fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit) {
            unsigned int n = mesh.valence(*fit);
            OpenGP::SurfaceMesh::Vertex_around_face_circulator vit = mesh.vertices(*fit);
            for(unsigned int v = 0; v < n; ++v) {
                indices.push_back((*vit).idx());
                ++vit;
            }
        }

        GLuint _vbo_indices;
        glGenBuffers(ONE, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        check_error_gl();

        glUseProgram(_pid);

        _height_tex = generate_noise();

        GLuint tex_id = glGetUniformLocation(_pid, "height_map");
        check_error_gl();
        glUniform1i(tex_id, 0);
        check_error_gl();

        OpenGP::EigenImage<vec3> image;
        OpenGP::imread("grass.tga", image);

        glGenTextures(1, &_diffuse_tex);
        glBindTexture(GL_TEXTURE_2D, _diffuse_tex);

        check_error_gl();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        check_error_gl();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
                     image.cols(), image.rows(), 0,
                     GL_RGB, GL_FLOAT, image.data());
        check_error_gl();

        tex_id = glGetUniformLocation(_pid, "diffuse_map");
        glUniform1i(tex_id, 1);

        glUseProgram(0);

    }

    void draw(mat4 Model, mat4 View, mat4 Projection, vec3 light_pos, vec3 cam_pos, bool blend){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Positions
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        glEnableVertexAttribArray(vpoint_id);

        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _height_tex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _diffuse_tex);

        //https://learnopengl.com/#!Advanced-OpenGL/Blending
        if(blend){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
            glUniform1f(glGetUniformLocation(_pid, "BLEND"),1);
        } else {
            glUniform1f(glGetUniformLocation(_pid, "BLEND"),0);
        }

        ///--- Set the MVP to vshader
        glUniformMatrix4fv(glGetUniformLocation(_pid, "MODEL"), 1, GL_FALSE, Model.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "VIEW"), 1, GL_FALSE, View.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "PROJ"), 1, GL_FALSE, Projection.data());
        glUniform3fv(glGetUniformLocation(_pid, "LIGHT"),1, &light_pos[0]);
        glUniform3fv(glGetUniformLocation(_pid, "CAM"),1, &cam_pos[0]);
        float time = glfwGetTime();
                GLint loc = glGetUniformLocation(_pid,"time");
                if(loc != -1){
                    glUniform1f(loc, glfwGetTime());
                }


        check_error_gl();
        ///--- Draw
        glDrawElements(GL_TRIANGLES,
                    /*#vertices*/ 3*mesh.n_faces(),
                    GL_UNSIGNED_INT,
                    ZERO_BUFFER_OFFSET);
        check_error_gl();
        glDisable(GL_BLEND);

        ///--- Clean up
        glBindVertexArray(0);
        glUseProgram(0);
    }
};
