#include "path.h"
#include "../../../range/def.h"
#include <assert.h>
#include "../../../window/def.h"
#include "../../../convert/type.h"
#include <stdlib.h>
#include "../../../convert/source.h"
#include "../../../convert/fd/source.h"
#include <fcntl.h>
#include <stdint.h>
#include "../../../gltf/type.h"
#include "../../../gltf/load/source.h"
#include "../../../gltf/copy.h"
#include "../../../range/alloc.h"
#include <stdio.h>
#include "../../../window/alloc.h"
#include "../../../log/log.h"
#include "glb.h"
#include "../../../gltf/free.h"

void load_glb_path (glb * target, const char * path)
{
    static window_unsigned_char buffer = {0};
    fd_source fd_source = fd_source_init(open(path, O_RDONLY), &buffer);
    assert (fd_source.fd >= 0);
    assert (glb_load_source(target, &fd_source.source));
    //glb_toc_copy_mem(&target->toc);
    glb_copy_mem(target);
    convert_source_clear(&fd_source.source);
    window_rewrite(buffer);
}

bool draw_buffer_load_path (draw_buffer ** result, int argc, char * const * argv)
{
    window_unsigned_char contents = {0};
    range_glb mesh_range = {0};
    range_calloc(mesh_range, argc);

    fd_source fd_source = { .fd = -1 };

    glb * i;
    
    for_range(i, mesh_range)
    {
	window_rewrite(contents);
	
	fd_source = fd_source_init(open(argv[range_index(i, mesh_range)], O_RDONLY),  &contents);

	if (fd_source.fd < 0)
	{
	    perror(argv[range_index(i, mesh_range)]);
	    log_fatal ("Could not open %f", argv);
	}

	if (!glb_load_source(i, &fd_source.source))
	{
	    log_fatal("Could not parse glb from the file at %f", argv);
	}

	glb_copy_mem(i);
    }

    if (!draw_buffer_load_glb(result, &mesh_range.alias_const))
    {
	log_fatal("Could not create buffer from meshes");
    }

    for_range(i, mesh_range)
    {
	glb_clear(i);
    }
    
    range_clear(mesh_range);

    window_clear(contents);
    
    return true;

fail:

    mesh_range.end = i;

    for_range(i, mesh_range)
    {
	glb_clear(i);
    }
    
    if (fd_source.fd > 0)
    {
	convert_source_clear(&fd_source.source);
    }
    
    window_clear(contents);
    range_clear(mesh_range);

    return false;
}
