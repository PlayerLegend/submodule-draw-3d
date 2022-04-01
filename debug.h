#ifndef FLAT_INCLUDES
#define FLAT_INCLUDES
#include "../glad/glad.h"
#include <assert.h>
#include "../log/log.h"
#include <stdbool.h>
#endif

#ifndef NDEBUG
#define gl_error_stringize(value) #value
#define gl_error_stringize2(value) gl_error_stringize(value)
#define gl_error_case(value) \
    case value:\
    log_error("GL Error in %s: " #value, name);	\
    break;

static void _gl_check(const char * name, GLint error)
{
    if (error != GL_NO_ERROR)
    {
	switch (error)
	{
	    gl_error_case(GL_INVALID_VALUE);
	    gl_error_case(GL_INVALID_OPERATION);
	}

	assert (false);
    }
}

#define gl_check(name) _gl_check(name, glGetError())    
#else
#endif
