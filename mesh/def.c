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

void mesh_transfer (draw_mesh * to, draw_mesh * from)
{
    draw_mesh_instance ** i;

    range_draw_mesh_instance_p instances = from->instances.region;

    window_rewrite(from->instances);

    window_alloc(to->instances, range_count(to->instances.region) + range_count(instances));

    for_range (i, instances)
    {
	(*i)->mesh = to;
	*window_push(to->instances) = *i;
    }
}
