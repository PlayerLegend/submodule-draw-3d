#include "def.h"
#include "internal/def.h"

void draw_buffer_mesh_access (range_draw_mesh * result, draw_buffer * target)
{
    result->begin = target->meshes;
    result->end = result->begin + target->mesh_count;
}
