#ifndef FLAT_INCLUDES
#include <stdbool.h>
#include <unistd.h>
#include "../../../glad/include/glad/glad.h"
#define FLAT_INCLUDES
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../vec/vec.h"
#include "../../vec/vec3.h"
#include "../../vec/vec4.h"
#include "../../keyargs/keyargs.h"
#include "../../json/def.h"
#include "../../gltf/def.h"
#endif

typedef struct draw_mesh draw_mesh;

typedef struct {
    GLuint begin;
    GLuint end;
}
    draw_mesh_index_range;

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    float scale;
    draw_mesh * parent;
}
    draw_mesh_instance;

range_typedef(draw_mesh, draw_mesh);
range_typedef(draw_mesh_instance*, draw_mesh_instance_p);
window_typedef(draw_mesh_instance*, draw_mesh_instance_p);

struct draw_mesh {
    draw_mesh_index_range index;
    window_draw_mesh_instance_p instances;
};

void mesh_instance_set_mesh (draw_mesh_instance * instance, draw_mesh * mesh);