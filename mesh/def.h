#ifndef FLAT_INCLUDES
#include "../../gltf/def.h"
#include "../../vec/vec.h"
#include "../../vec/vec3.h"
#include "../../vec/vec4.h"
#include "../../glad/glad.h"
#include "../../vec/object3.h"
#endif

typedef struct draw_mesh draw_mesh;

typedef struct {
    GLuint begin;
    GLuint end;
}
    draw_mesh_index_range;

typedef struct {
    vec_object3 * origin;
    draw_mesh * mesh;
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
void mesh_transfer (draw_mesh * to, draw_mesh * from);
