#ifndef FLAT_INCLUDES
#include "../../vec/vec4.h"
#endif

typedef struct draw_cache draw_cache;
typedef struct draw_buffer draw_buffer;

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    float fov;
}
    draw_view;
