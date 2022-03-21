#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../../../glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#define FLAT_INCLUDES
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../vec/vec.h"
#include "../../vec/vec3.h"
#include "../../vec/vec4.h"
#include "../../keyargs/keyargs.h"
#include "../../vec/mat4.h"
#include "../../json/def.h"
#include "../../gltf/def.h"
#include "../mesh/def.h"
#include "def.h"
#include "internal/def.h"
#include "draw.h"
#include "../../log/log.h"

void draw_buffer_draw (draw_buffer * buffer, draw_view * view, int shader)
{
    range_typedef(draw_mesh, draw_mesh);

    range_draw_mesh meshes = { .begin = buffer->meshes, .end = buffer->meshes + buffer->mesh_count };

    draw_mesh * mesh;

    draw_mesh_instance ** instance;
    
    glBindBuffer (GL_ARRAY_BUFFER, buffer->vbo);
    glBindVertexArray(buffer->vao);

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
	.model_translation = glGetUniformLocation(shader, "uniform_model_translation"),
	.model_rotation = glGetUniformLocation(shader, "uniform_model_rotation"),
	.model_transform = glGetUniformLocation(shader, "uniform_model_transform"),
	.model_scale = glGetUniformLocation(shader, "uniform_model_scale"),

	.view_translation = glGetUniformLocation(shader, "uniform_view_translation"),
	.view_rotation = glGetUniformLocation(shader, "uniform_view_rotation"),
	.view_transform = glGetUniformLocation(shader, "uniform_view_transform"),

	.projection = glGetUniformLocation(shader, "uniform_projection"),
	
    	.mvp_rotation = glGetUniformLocation(shader, "uniform_mvp_rotation"),
	.mvp_transform = glGetUniformLocation(shader, "uniform_mvp_transform"),
    };
    
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

    GLuint time_location = glGetUniformLocation (shader, "uniform_time");

    mat4_setup_translation_matrix(.result = &matrix.view.translation, .translation = view->position);
    mat4_setup_rotation_matrix(.result = &matrix.view.rotation, .quaternion = &view->quaternion);
    mat4_multiply (&matrix.view.transform, &matrix.view.rotation, &matrix.view.translation);

    glUniformMatrix4fv(matrix_id.view_rotation, 1, GL_FALSE, matrix.view.rotation.index);
    glUniformMatrix4fv(matrix_id.view_translation, 1, GL_FALSE, matrix.view.translation.index);
    glUniformMatrix4fv(matrix_id.view_transform, 1, GL_FALSE, matrix.view.transform.index);

    mat4_setup_projection_matrix(.result = &matrix.projection,
				 .fovy = 2.0 * 3.14159 / (3.0 * 2),
				 .aspect = 1,
				 .near = 0.01,
				 .far = 1000);
    glUniformMatrix4fv(matrix_id.projection, 1, GL_FALSE, matrix.projection.index);
    
    for_range (mesh, meshes)
    {
	for_range (instance, mesh->instances.region)
	{
	    mat4_setup_scale_matrix(.result = &matrix.scale, .scale = (fvec3) { .x = (*instance)->scale, .y = (*instance)->scale, .z = (*instance)->scale });
    	    mat4_setup_translation_matrix (.result = &matrix.model.translation, .translation = (*instance)->position);
	    mat4_setup_rotation_matrix (.result = &matrix.model.rotation, .quaternion = &(*instance)->quaternion);

	    mat4_multiply (&matrix.tmp, &matrix.model.rotation, &matrix.scale);
	    mat4_multiply (&matrix.model.transform, &matrix.tmp, &matrix.model.translation);
	    
	    mat4_multiply (&matrix.mvp.rotation, &matrix.view.rotation, &matrix.model.rotation);
	    mat4_multiply (&matrix.tmp, &matrix.view.transform, &matrix.model.transform);
	    mat4_multiply (&matrix.mvp.transform, &matrix.projection, &matrix.tmp);
	    
	    glUniformMatrix4fv(matrix_id.model_rotation, 1, GL_FALSE, matrix.model.rotation.index);
	    glUniformMatrix4fv(matrix_id.model_translation, 1, GL_FALSE, matrix.model.translation.index);
	    glUniformMatrix4fv(matrix_id.model_transform, 1, GL_FALSE, matrix.model.transform.index);
	    glUniformMatrix4fv(matrix_id.model_scale, 1, GL_FALSE, matrix.scale.index);
    
	    glUniformMatrix4fv(matrix_id.mvp_rotation, 1, GL_FALSE, matrix.mvp.rotation.index);
	    glUniformMatrix4fv(matrix_id.mvp_transform, 1, GL_FALSE, matrix.mvp.transform.index);

	    glUniform1f (time_location, glfwGetTime());
	    
	    glDrawArrays (GL_TRIANGLES, mesh->index.begin, mesh->index.end);
	}
    }
}
