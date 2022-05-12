#include "loader.h"
#include "../../gltf/parse.h"
#include <assert.h>
#include "../../window/alloc.h"
#include "../../gltf/env.h"
#include "../../convert/status.h"
#include "../../convert/source.h"
#include "../../gltf/convert.h"
#include "../../log/log.h"
#include "internal/def.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

range_typedef(GLuint, GLuint);
window_typedef(GLuint, GLuint);

typedef vec3(GLfloat) GLfloat3;
range_typedef(GLfloat3, GLfloat3);
window_typedef(GLfloat3, GLfloat3);

typedef struct {
    window_GLuint start_index;
    window_gltf_index indices;
    window_GLfloat3 position;
    window_GLfloat3 normal;
}
    vbo_loader_buffers;

bool GLfloat3_loader(void * target_void, const fvec3 * input)
{
    window_GLfloat3 * target = target_void;

    *window_push (*target) = (GLfloat3) { .x = input->x, .y = input->y, .z = input->z };

    return true;
}

static bool add_primitive_position (window_GLfloat3 * position, range_gltf_index * indices, const glb_toc * toc, gltf_mesh_primitive * primitive)
{
    gltf_accessor_env env;

    gltf_accessor_env_setup (&env, toc, primitive->attributes.position);

    return gltf_accessor_env_load_fvec3(position, GLfloat3_loader, indices, &env);
}

static bool add_primitive_normal (window_GLfloat3 * normal, range_gltf_index * indices, const glb_toc * toc, gltf_mesh_primitive * primitive)
{
    gltf_accessor_env env;

    gltf_accessor_env_setup (&env, toc, primitive->attributes.normal);

    return gltf_accessor_env_load_fvec3(normal, GLfloat3_loader, indices, &env);
}

static bool add_primitive (vbo_loader_buffers * buffers, const glb_toc * toc, gltf_mesh_primitive * primitive)
{
    gltf_accessor_env env;

    gltf_accessor_env_setup(&env, toc, primitive->indices);

    window_rewrite (buffers->indices);
    gltf_accessor_env_load_indices (&buffers->indices, &env);

    if (!add_primitive_position (&buffers->position, &buffers->indices.region, toc, primitive))
    {
	log_fatal ("Failed to add primitive position");
    }

    if (!add_primitive_normal (&buffers->normal, &buffers->indices.region, toc, primitive))
    {
	log_fatal ("Failed to add primitive normal");
    }
    
    return true;

fail:
    return false;
}

static bool add_gltf_mesh (vbo_loader_buffers * buffers, const glb_toc * toc, gltf_mesh * mesh)
{
    gltf_mesh_primitive * primitive;

    for_range (primitive, mesh->primitives)
    {
	if (!add_primitive (buffers, toc, primitive))
	{
	    log_fatal ("Failed to add primitive %zu/%zu", range_index(primitive, mesh->primitives), range_count(mesh->primitives));
	}
    }

    return true;
fail:
    return false;
}

static bool add_glb_to_buffers (vbo_loader_buffers * buffers, const glb * glb)
{
    *window_push(buffers->start_index) = range_count(buffers->position.region);

    gltf_mesh * mesh;

    for_range (mesh, glb->gltf.meshes)
    {
	if (!add_gltf_mesh (buffers, &glb->toc, mesh))
	{
	    log_fatal ("Failed to add mesh %d", range_index(mesh, glb->gltf.meshes));
	}
    }

    return true;

fail:
    return false;
}

static void free_buffers(vbo_loader_buffers * buffers)
{
    free (buffers->start_index.alloc.begin);
    free (buffers->indices.alloc.begin);
    free (buffers->position.alloc.begin);
    free (buffers->normal.alloc.begin);
}

static bool load_vbo_loader_buffers (vbo_loader_buffers * buffers, range_const_glb * input)
{
    const glb * i;
    for_range(i, *input)
    {
	if (!add_glb_to_buffers (buffers, i))
	{
	    return false;
	}
    }

    return true;
}

bool draw_buffer_load_batch (draw_buffer ** result, range_const_glb * input)
{
    assert (glGetError() == GL_NO_ERROR);
    
    vbo_loader_buffers buffers = {0};

    unsigned char * vbo_contents = NULL;
    
    if (!load_vbo_loader_buffers (&buffers, input))
    {
	log_fatal ("Could not load a specified mesh uri for drawing");
    }
    
    size_t position_buffer_size = range_count (buffers.position.region) * sizeof(*buffers.position.region.begin);
    size_t normal_buffer_size = range_count (buffers.normal.region) * sizeof(*buffers.normal.region.begin);

    size_t vbo_size = position_buffer_size + normal_buffer_size;

    vbo_contents = calloc (1, vbo_size);

    size_t input_count = range_count(*input);

    memcpy (vbo_contents, buffers.position.region.begin, position_buffer_size);
    memcpy (vbo_contents + position_buffer_size, buffers.normal.region.begin, normal_buffer_size);
    
    draw_buffer * buffer = calloc (1, sizeof(*buffer) + input_count * sizeof(draw_mesh));

    assert (input_count == (size_t)range_count (buffers.start_index.region));

    buffer->mesh_count = input_count;
    buffer->vertex_count = range_count (buffers.position.region);
    
    GLuint * index;
    for_range (index, buffers.start_index.region)
    {
	buffer->meshes[range_index(index, buffers.start_index.region)] = (draw_mesh)
	{
	    .index = { .begin = *index, .end = index + 1 < buffers.start_index.region.end ? index[1] : buffer->vertex_count },
	};
    }
    
    glGenBuffers (1, &buffer->vbo);
    glGenVertexArrays (1, &buffer->vao);
    glBindVertexArray(buffer->vao);
    
    glBindBuffer (GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData (GL_ARRAY_BUFFER, vbo_size, vbo_contents, GL_STATIC_DRAW);
    
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray (0);
    //glVertexAttribPointer (1, 3, GL_FLOAT, DRAW_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    //glVertexAttribPointer (1, 3, GL_FLOAT, DRAW_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)position_buffer_size);
    glEnableVertexAttribArray (1);
    
    assert (glGetError() == GL_NO_ERROR);

    free_buffers (&buffers);
    free (vbo_contents);

    *result = buffer;

    return true;

fail:
    free (vbo_contents);
    free_buffers (&buffers);
    return false;
}

void draw_buffer_free (draw_buffer * target)
{
    glDeleteVertexArrays (1, &target->vao);
    glDeleteBuffers (1, &target->vbo);

    struct range(draw_mesh) meshes = { .begin = target->meshes, .end = target->meshes + target->mesh_count };
    draw_mesh * mesh;
    draw_mesh_instance ** instance;

    for_range (mesh, meshes)
    {
	for_range (instance, mesh->instances.region)
	{
	    (*instance)->mesh = NULL;
	}
	free (mesh->instances.alloc.begin);
    }

    free (target);
}
