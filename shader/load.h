#ifndef FLAT_INCLUDES
#include <stdbool.h>
#include "../../convert/source.h"
#include "../../keyargs/keyargs.h"
#include "def.h"
#endif

keyargs_declare(bool,
		shader_load_path,
		shader_program * result;
		const char * vertex_path;
		const char * fragment_path;);

#define shader_load_path(...) keyargs_call(shader_load_path, __VA_ARGS__)
//GLuint load_shader_program (int vertex_fd, int fragment_fd);
