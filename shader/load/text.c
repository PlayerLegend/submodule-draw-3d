#include "text.h"
#include "../../../glad/glad.h"
#include "../../../log/log.h"
#include <stdlib.h>
#include <assert.h>

static bool _build_shader_from_string (GLint * result, GLenum shader_type, const char * file_contents)
{
    assert (glGetError() == GL_NO_ERROR);
    
    GLuint shader_id = glCreateShader (shader_type);

    glShaderSource (shader_id, 1, &file_contents, NULL);
    glCompileShader (shader_id);

    int info_log_length;

    GLint status = GL_FALSE;
    
    glGetShaderiv (shader_id, GL_COMPILE_STATUS, &status);
    glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0)
    {
	char * info_log = malloc (info_log_length + 1);
	glGetShaderInfoLog (shader_id, info_log_length, NULL, info_log);
	log_normal ("%s", info_log);
	free (info_log);
    }

    if (status != GL_TRUE)
    {
	log_fatal ("Failed to build the shader\nFile contents: %s", file_contents);
    }

    *result = shader_id;
    
    assert (glGetError() == GL_NO_ERROR);

    return true;

fail:

    return false;
}

bool draw_shader_vertex_load_text (draw_shader_vertex ** result, const char * input)
{
    *result = NULL;

    assert (sizeof(*result) >= sizeof(GLint));

    return _build_shader_from_string ( (GLint*) result, GL_VERTEX_SHADER, input );
}

bool draw_shader_fragment_load_text (draw_shader_fragment ** result, const char * input)
{
    *result = NULL;

    assert (sizeof(*result) >= sizeof(GLint));

    return _build_shader_from_string ( (GLint*) result, GL_FRAGMENT_SHADER, input );
}
