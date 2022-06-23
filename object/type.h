#ifndef FLAT_INCLUDES
#include "../../vec/object3.h"
#include "../../range/def.h"
#include "../../window/def.h"
#include "../mesh/type.h"
#endif

typedef struct {
    vec_object3 * origin;
    draw_mesh * mesh;
    bool hidden;
}
    draw_object;
