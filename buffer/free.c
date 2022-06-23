#include "type.internal.h"
#include "free.h"
#include <stdlib.h>

void draw_buffer_free (draw_buffer * target)
{
    glDeleteVertexArrays (1, &target->vao);
    glDeleteBuffers (1, &target->vbo);

    struct range(draw_mesh) meshes = { .begin = target->meshes, .end = target->meshes + target->mesh_count };
    draw_mesh * mesh;
    draw_object ** object;

    for_range (mesh, meshes)
    {
	for_range (object, mesh->instances.region)
	{
	    (*object)->mesh = NULL;
	}
	free (mesh->instances.alloc.begin);
    }

    free (target);
}
