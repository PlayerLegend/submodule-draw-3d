#ifndef FLAT_INCLUDES
#include "def.h"
#include "../shader/def.h"
#endif

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    float fov;
}
    draw_view;

void draw_buffer_draw (draw_buffer * buffer, draw_view * view, shader_program shader);
