#ifndef FLAT_INCLUDES
#include "../../mesh/def.h"
#endif

struct draw_buffer {
    GLuint vbo;
    GLuint vao;
    size_t mesh_count;
    size_t vertex_count;
    draw_mesh meshes[];
};
