#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "../../../glad/include/glad/glad.h"
#define FLAT_INCLUDES
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../vec/vec.h"
#include "../../vec/vec3.h"
#include "../../vec/vec4.h"
#include "../../keyargs/keyargs.h"
#include "../../gltf/def.h"
#include "def.h"
#include "../../window/alloc.h"

void mesh_instance_set_mesh (draw_mesh_instance * instance, draw_mesh * mesh)
{
    if (instance->parent)
    {
	assert (!range_is_empty(instance->parent->instances.region));

	draw_mesh_instance ** i;
	for_range(i, instance->parent->instances.region)
	{
	    if (*i == instance)
	    {
		*i = *(--instance->parent->instances.region.end);
		break;
	    }
	}
    }
    
    *window_push(mesh->instances) = instance;
    instance->parent = mesh;
}
