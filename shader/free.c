#include "free.h"
#include <assert.h>
#include "../../glad/glad.h"

void draw_shader_vertex_free(draw_shader_vertex * target)
{
    assert (glGetError() == GL_NO_ERROR);
    
    glDeleteShader ((intptr_t)target);
    
    assert (glGetError() == GL_NO_ERROR);
}

void draw_shader_fragment_free(draw_shader_fragment * target)
{
    assert (glGetError() == GL_NO_ERROR);
    
    glDeleteShader ((intptr_t)target);
    
    assert (glGetError() == GL_NO_ERROR);
}

void draw_shader_program_free(draw_shader_program * target)
{
    assert (glGetError() == GL_NO_ERROR);
    
    glDeleteProgram ((intptr_t)target);
    
    assert (glGetError() == GL_NO_ERROR);
}
