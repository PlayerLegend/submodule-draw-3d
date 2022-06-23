#include "set.h"
#include "../../glad/glad.h"
#include "../mesh/type.internal.h"
#include "../../window/alloc.h"

void draw_object_set_mesh(draw_object * target, draw_mesh * mesh)
{
    assert(target);
    
    if (target->mesh)
    {
	assert (!range_is_empty(target->mesh->instances.region));

	draw_object ** i;
	for_range(i, target->mesh->instances.region)
	{
	    if (*i == target)
	    {
		*i = *(--target->mesh->instances.region.end);
		break;
	    }
	}
    }

    if (mesh)
    {
	*window_push(mesh->instances) = target;
    }
    
    target->mesh = mesh;
}
