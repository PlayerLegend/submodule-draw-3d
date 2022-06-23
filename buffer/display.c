#include "display.h"
#include <stdbool.h>
#include "../../vec/shape3.h"
#include "../../vec/object3.h"
#include <assert.h>
#include "../../glad/glad.h"
#include "../../range/def.h"
#include "../../window/def.h"
#include "../mesh/type.h"
#include "../object/type.h"
#include "../mesh/type.internal.h"
#include "type.internal.h"
#include "../../keyargs/keyargs.h"
#include "../../vec/mat4.h"
#include "../../game-interface/time.h"
#include "../../log/log.h"

void draw_buffer_display (const draw_buffer * buffer, const draw_view * view, const draw_shader_program * shader)
{
    glUseProgram((intptr_t)shader);
    
    assert (glGetError() == GL_NO_ERROR);

    range_const_draw_mesh meshes = { .begin = buffer->meshes, .end = buffer->meshes + buffer->mesh_count };

    glBindBuffer (GL_ARRAY_BUFFER, buffer->vbo);

    assert (glGetError() == GL_NO_ERROR);
    
    glBindVertexArray(buffer->vao);

    assert (glGetError() == GL_NO_ERROR);
    
    struct {
	int model_translation;
	int model_rotation;
	int model_transform;
	int model_scale;
	int view_translation;
	int view_rotation;
	int view_transform;
	int projection;
	int mvp_rotation;
	int mvp_transform;
    }
	matrix_id =
    {
	.model_translation = glGetUniformLocation((intptr_t)shader, "uniform_model_translation"),
	.model_rotation = glGetUniformLocation((intptr_t)shader, "uniform_model_rotation"),
	.model_transform = glGetUniformLocation((intptr_t)shader, "uniform_model_transform"),
	.model_scale = glGetUniformLocation((intptr_t)shader, "uniform_model_scale"),

	.view_translation = glGetUniformLocation((intptr_t)shader, "uniform_view_translation"),
	.view_rotation = glGetUniformLocation((intptr_t)shader, "uniform_view_rotation"),
	.view_transform = glGetUniformLocation((intptr_t)shader, "uniform_view_transform"),

	.projection = glGetUniformLocation((intptr_t)shader, "uniform_projection"),
	
    	.mvp_rotation = glGetUniformLocation((intptr_t)shader, "uniform_mvp_rotation"),
	.mvp_transform = glGetUniformLocation((intptr_t)shader, "uniform_mvp_transform"),
    };

    assert (glGetError() == GL_NO_ERROR);
    
    struct {
	mat4 scale;
	
	struct {
	    mat4 translation;
	    mat4 rotation;
	    mat4 transform;
	} model, view;

	struct {
	    mat4 rotation;
	    mat4 transform;
	}
	    mvp;

	mat4 projection;

	mat4 tmp;
    }
	matrix;

    GLuint time_location = glGetUniformLocation ((intptr_t)shader, "uniform_time");

    assert (glGetError() == GL_NO_ERROR);

    mat4_setup_translation_matrix(.result = &matrix.view.translation, .translation = (fvec3){ vec3_negate_init(view->position) });
    mat4_setup_rotation_matrix(.result = &matrix.view.rotation, .quaternion = &view->quaternion);
    mat4_multiply (&matrix.view.transform, &matrix.view.rotation, &matrix.view.translation);

    glUniformMatrix4fv(matrix_id.view_rotation, 1, GL_FALSE, matrix.view.rotation.index);
    assert (glGetError() == GL_NO_ERROR);

    glUniformMatrix4fv(matrix_id.view_translation, 1, GL_FALSE, matrix.view.translation.index);
    assert (glGetError() == GL_NO_ERROR);
    
    glUniformMatrix4fv(matrix_id.view_transform, 1, GL_FALSE, matrix.view.transform.index);
    assert (glGetError() == GL_NO_ERROR);

    mat4_setup_projection_matrix(.result = &matrix.projection,
				 .fovy = 2.0 * 3.14159 / (3.0 * 2),
				 .aspect = 1,
				 .near = 0.01,
				 .far = 1000);
    glUniformMatrix4fv(matrix_id.projection, 1, GL_FALSE, matrix.projection.index);
    assert (glGetError() == GL_NO_ERROR);

    const draw_mesh * mesh;
    draw_object * const * object;
    
    for_range (mesh, meshes)
    {
	for_range (object, mesh->instances.region)
	{
	    if ( (*object)->hidden )
	    {
		continue;
	    }
	    
	    mat4_setup_scale_matrix(.result = &matrix.scale, .scale = (*object)->origin->scale);
    	    mat4_setup_translation_matrix (.result = &matrix.model.translation, .translation = (*object)->origin->position);
	    mat4_setup_rotation_matrix (.result = &matrix.model.rotation, .quaternion = &(*object)->origin->quaternion);

	    mat4_multiply (&matrix.tmp, &matrix.model.translation, &matrix.model.rotation);
	    mat4_multiply (&matrix.model.transform, &matrix.tmp, &matrix.scale);
	    
	    mat4_multiply (&matrix.mvp.rotation, &matrix.view.rotation, &matrix.model.rotation);
	    mat4_multiply (&matrix.tmp, &matrix.view.transform, &matrix.model.transform);
	    mat4_multiply (&matrix.mvp.transform, &matrix.projection, &matrix.tmp);
	    
	    glUniformMatrix4fv(matrix_id.model_rotation, 1, GL_FALSE, matrix.model.rotation.index);
	    glUniformMatrix4fv(matrix_id.model_translation, 1, GL_FALSE, matrix.model.translation.index);
	    glUniformMatrix4fv(matrix_id.model_transform, 1, GL_FALSE, matrix.model.transform.index);
	    glUniformMatrix4fv(matrix_id.model_scale, 1, GL_FALSE, matrix.scale.index);
    
	    glUniformMatrix4fv(matrix_id.mvp_rotation, 1, GL_FALSE, matrix.mvp.rotation.index);
	    glUniformMatrix4fv(matrix_id.mvp_transform, 1, GL_FALSE, matrix.mvp.transform.index);

	    glUniform1f (time_location, ui_get_time());

	    assert (glGetError() == GL_NO_ERROR);
	    glDrawArrays (GL_TRIANGLES, mesh->index.begin, mesh->index.end - mesh->index.begin);
	    assert (glGetError() == GL_NO_ERROR);
	}
    }
}
