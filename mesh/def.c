#include "def.h"
#include <assert.h>
#include "../../window/alloc.h"

void mesh_instance_set_mesh (draw_mesh_instance * instance, draw_mesh * mesh)
{
    if (instance->mesh)
    {
	assert (!range_is_empty(instance->mesh->instances.region));

	draw_mesh_instance ** i;
	for_range(i, instance->mesh->instances.region)
	{
	    if (*i == instance)
	    {
		*i = *(--instance->mesh->instances.region.end);
		break;
	    }
	}
    }
    
    *window_push(mesh->instances) = instance;
    instance->mesh = mesh;
}
