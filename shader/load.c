#include "load.h"
#include "../../glad/glad.h"
#include "../../convert/fd/source.h"
#include <stdlib.h>
#include "../../log/log.h"
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include "../../window/alloc.h"
#include <stdlib.h>
#include <unistd.h>
#include "../debug.h"

static bool _build_shader_from_string (GLint * result, GLenum shader_type, const char * file_contents)
{
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
	log_fatal ("Failed to build the shader");
    }

    *result = shader_id;

    return true;

fail:
    return false;
}

static bool _build_shader_from_source (GLint * result, GLenum shader_type, convert_source * source)
{
    if (!convert_load_all (source))
    {
	log_fatal ("Failed to load shader from source");
    }

    return _build_shader_from_string (result, shader_type, source->contents->region.char_cast.const_cast.begin);

fail:
    return false;
}

static bool _build_shader_from_fd (GLint * result, GLenum shader_type, int fd)
{
    assert (fd >= 0);
    window_unsigned_char file_contents = {0};
    fd_source fd_source = fd_source_init (fd, &file_contents);
    
    bool status = _build_shader_from_source (result, shader_type, &fd_source.source);

    window_clear (file_contents);

    return status;
}

static bool _build_shader_from_path (GLint * result, GLenum shader_type, const char * path)
{
    int fd = open (path, O_RDONLY);

    if (fd < 0)
    {
	perror(path);
	log_fatal ("Failed to load shader from path %s", path);
    }

    bool status = _build_shader_from_fd (result, shader_type, fd);

    close (fd);
    
    return status;
    
fail:
    return false;
}

static bool _link_shader_program (GLint * result, GLint vertex_id, GLint fragment_id)
{
    gl_check("startup");
    
    GLuint program_id = glCreateProgram();

    gl_check("glCreateProgram");

    assert (vertex_id != fragment_id);
    
    glAttachShader (program_id, vertex_id);
    gl_check("glAttachShader vertex_id");
    
    glAttachShader (program_id, fragment_id);
    gl_check("glAttachShader fragment_id");
    
    glLinkProgram (program_id);
    
    assert (glGetError() == GL_NO_ERROR);

    int info_log_length;

    GLint status;
    
    glGetProgramiv (program_id, GL_LINK_STATUS, &status);
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

    *result = program_id;

    assert (glGetError() == GL_NO_ERROR);
    
    return true;

fail:
    return false;
}

keyargs_define(shader_load_path)
{
    assert (glGetError() == GL_NO_ERROR);
    
    GLint vertex_id = GL_FALSE;
    GLint fragment_id = GL_FALSE;
    
    if (!_build_shader_from_path (&vertex_id, GL_VERTEX_SHADER, args.vertex_path))
    {
	log_fatal ("Failed to build vertex shader");
    }

    assert (glGetError() == GL_NO_ERROR);
    
    if (!_build_shader_from_path (&fragment_id, GL_FRAGMENT_SHADER, args.fragment_path))
    {
	log_fatal ("Failed to build fragment shader");
    }

    assert (glGetError() == GL_NO_ERROR);
    
    GLint program_id;

    if (!_link_shader_program(&program_id, vertex_id, fragment_id))
    {
	log_fatal ("Failed to link the shader program");
    }
    
    assert (glGetError() == GL_NO_ERROR);
    
    glDeleteShader (vertex_id);
    glDeleteShader (fragment_id);

    assert (sizeof(*args.result) >= sizeof(program_id));

    *args.result = program_id;
    
    assert (glGetError() == GL_NO_ERROR);

    glUseProgram(program_id);

    return true;
    
fail:
    if (vertex_id != GL_FALSE)
    {
	glDeleteShader (vertex_id);
    }

    if (fragment_id != GL_FALSE)
    {
	glDeleteShader (fragment_id);
    }

    return false;
}

/*
GLuint load_shader_program (int vertex_fd, int fragment_fd)
{
    window_unsigned_char file_contents = {0};

    fd_source fd_source = fd_source_init (args.vertex_fd, &file_contents);

    log_normal ("Building vertex shader");

    if (!convert_load_all(&fd_source.source))
    {
	log_fatal ("Failed to read vertex shader");
    }

    //log_normal ("Shader contents: %.*s", range_count(file_contents.region), file_contents.region.begin);
    
    GLuint vertex_id = _build_shader (file_contents.signed_cast.region.begin, GL_VERTEX_SHADER);

    if (vertex_id == GL_FALSE)
    {
	goto fail;
    }

    log_normal ("Building fragment shader");

    convert_source_clear(&fd_source.source);

    fd_source = fd_source_init (args.fragment_fd, &file_contents);

    window_rewrite(file_contents);
    
    if (!convert_load_all(&fd_source.source))
    {
	log_error ("Failed to read fragment shader");
	goto fail;
    }

    //log_normal ("Shader contents: %.*s", range_count(file_contents.region), file_contents.region.begin);

    GLuint fragment_id = _build_shader (file_contents.signed_cast.region.begin, GL_FRAGMENT_SHADER);
    
    if (fragment_id == GL_FALSE)
    {
	goto fail;
    }

    log_normal ("Linking shader program");
    GLuint program_id = glCreateProgram();
    glAttachShader (program_id, vertex_id);
    glAttachShader (program_id, fragment_id);
    glLinkProgram (program_id);

    GLint result;
    int info_log_length;
    
    glGetProgramiv (program_id, GL_LINK_STATUS, &result);
    glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0)
    {
	char * info_log = malloc (info_log_length + 1);
	glGetProgramInfoLog (program_id, info_log_length, NULL, info_log);
	log_normal ("%s", info_log);
	free (info_log);
    }

    if (result != GL_TRUE)
    {
	log_error ("Failed to link shader program");
	goto fail;
    }

    glDetachShader (program_id, vertex_id);
    glDetachShader (program_id, fragment_id);

    glDeleteShader (vertex_id);
    glDeleteShader (fragment_id);

    free (file_contents.alloc.begin);
    convert_source_clear(&fd_source.source);

    return program_id;

fail:
    convert_source_clear(&fd_source.source);
    free (file_contents.alloc.begin);
    return GL_FALSE;
}
*/
