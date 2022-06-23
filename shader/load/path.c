#include "source.h"
#include "path.h"
#include "../../../convert/fd/source.h"
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include "../../../window/alloc.h"

bool draw_shader_vertex_load_path (draw_shader_vertex ** result, const char * input)
{
    window_unsigned_char contents = {0};
    fd_source fd_source = fd_source_init(open(input, O_RDONLY), &contents);

    if (fd_source.fd < 0)
    {
	perror(input);
	return false;
    }

    bool retval = draw_shader_vertex_load_source(result, &fd_source.source);

    convert_source_clear(&fd_source.source);

    window_clear(contents);

    return retval;
}

bool draw_shader_fragment_load_path (draw_shader_fragment ** result, const char * input)
{
    window_unsigned_char contents = {0};
    fd_source fd_source = fd_source_init(open(input, O_RDONLY), &contents);

    if (fd_source.fd < 0)
    {
	perror(input);
	return false;
    }

    bool retval = draw_shader_fragment_load_source(result, &fd_source.source);

    convert_source_clear(&fd_source.source);

    window_clear(contents);
    
    return retval;
}
