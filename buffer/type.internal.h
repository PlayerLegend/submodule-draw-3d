#ifndef FLAT_INCLUDES
#include "type.h"
#include "../../glad/glad.h"
#include <stddef.h>
#include <stdbool.h>
#include "../mesh/type.h"
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../vec/shape3.h"
#include "../../vec/object3.h"
#include "../object/type.h"
#include "../mesh/type.internal.h"
#endif

struct draw_buffer {
    GLuint vbo;
    GLuint vao;
    size_t mesh_count;
    size_t vertex_count;
    draw_mesh meshes[];
};
