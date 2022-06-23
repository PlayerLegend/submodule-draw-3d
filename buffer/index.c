#include "index.h"
#include "../../glad/glad.h"
#include <stddef.h>
#include <stdbool.h>
#include "../mesh/type.h"
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../vec/shape3.h"
#include "../../vec/object3.h"
#include "../object/type.h"
#include "../mesh/type.internal.h"
#include "type.internal.h"
#include <assert.h>

draw_mesh * draw_buffer_index (draw_buffer * buffer, unsigned int index)
{
    assert (index < buffer->mesh_count);

    if (index >= buffer->mesh_count)
    {
	return false;
    }
    
    return buffer->meshes + index;
}
