#include "link.h"
#include <assert.h>
#include "../../glad/glad.h"
#include "../../log/log.h"
#include <stdlib.h>

bool draw_shader_program_link(draw_shader_program ** result, const draw_shader_vertex * vertex, const draw_shader_fragment * fragment)
{
    assert (glGetError() == GL_NO_ERROR);

    intptr_t vertex_id = (intptr_t) vertex;
    intptr_t fragment_id = (intptr_t) fragment;
    
    intptr_t program_id = glCreateProgram();

    assert (vertex_id != fragment_id);
    
    glAttachShader (program_id, vertex_id);
    assert (glGetError() == GL_NO_ERROR);
    
    glAttachShader (program_id, fragment_id);
    assert (glGetError() == GL_NO_ERROR);
    
    glLinkProgram (program_id);
    assert (glGetError() == GL_NO_ERROR);

    int info_log_length;

    GLint status;
    
    glGetProgramiv (program_id, GL_LINK_STATUS, &status);
    assert (glGetError() == GL_NO_ERROR);
    
    glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    assert (glGetError() == GL_NO_ERROR);
    
    if (info_log_length > 0)
    {
	char * info_log = malloc (info_log_length + 1);
	glGetProgramInfoLog (program_id, info_log_length, NULL, info_log);
	log_normal ("Shader log: %s", info_log);
	free (info_log);
    }
    else
    {
	log_normal ("Shader gave no log");
    }

    assert (glGetError() == GL_NO_ERROR);
    
    if (status != GL_TRUE)
    {
	log_fatal ("Failed to link shader program");
    }

    glDetachShader (program_id, vertex_id);
    glDetachShader (program_id, fragment_id);

    *(intptr_t*)result = program_id;

    assert (glGetError() == GL_NO_ERROR);
    
    return true;

fail:
    return false;
}
