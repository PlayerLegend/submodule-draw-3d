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
#include "../../vec/mat4.h"
#include "../../gltf/def.h"
#include "../mesh/def.h"
#include "def.h"
#endif

typedef struct {
    fvec3 position;
    fvec4 quaternion;
    float fov;
}
    draw_view;

void draw_buffer_draw (draw_buffer * buffer, draw_view * view, int shader);
