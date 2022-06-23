#ifndef FLAT_INCLUDES
#include "../../glad/glad.h"
#include "../object/type.h"
#endif

typedef struct {
    GLuint begin;
    GLuint end;
}
    draw_mesh_index_range;

range_typedef(draw_mesh, draw_mesh);
range_typedef(draw_object*, draw_object_p);
window_typedef(draw_object*, draw_object_p);

struct draw_mesh {
    draw_mesh_index_range index;
    window_draw_object_p instances;
};
