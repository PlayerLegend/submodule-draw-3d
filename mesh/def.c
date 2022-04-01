#include "def.h"
#include <assert.h>
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
