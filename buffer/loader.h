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
#include "../mesh/def.h"
#include "def.h"
#endif

draw_buffer * draw_buffer_load (int count, const char ** uris);

void draw_buffer_free (draw_buffer * target);
