#ifndef FLAT_INCLUDES
#include <stdbool.h>
#include <unistd.h>
#include "../../../../glad/include/glad/glad.h"
#define FLAT_INCLUDES
#include "../../../range/def.h"
#include "../../../window/def.h"
#include "../../../vec/vec.h"
#include "../../../vec/vec3.h"
#include "../../../vec/vec4.h"
#include "../../../json/def.h"
#include "../../../gltf/def.h"
#include "../../mesh/def.h"
#endif

struct draw_buffer {
    GLuint vbo;
    GLuint vao;
    size_t mesh_count;
    size_t vertex_count;
    draw_mesh meshes[];
};
