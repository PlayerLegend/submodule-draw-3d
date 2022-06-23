#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../../window/def.h"
#include "../../vec/vec.h"
#include "../../vec/vec3.h"
#include "../../vec/vec4.h"
#include "../buffer/type.h"
#include "../shader/type.h"
#include "../mesh/type.h"
#include "../../vec/shape3.h"
#include "../../vec/object3.h"
#include "../object/type.h"
#include "../object/set.h"
#include "../buffer/index.h"
#include "../../keyargs/keyargs.h"
#include "../../game-interface/init.h"
#include "../../game-interface/window.h"
#include "../../log/log.h"
#include "../../gltf/type.h"
#include "../buffer/display.h"
#include "../buffer/load/path.h"
#include "../shader/load/path.h"
#include "../shader/link.h"
#include "../shader/free.h"
#include "../buffer/free.h"
#include "../../game-interface/time.h"
#include "../../gltf/free.h"

int main(int argc, char * argv[])
{
    assert (argc == 4);
    
    assert (ui_init());

    ui_window * window;
    assert (ui_window_new(&window, .width = 640, .height = 480, .title = "draw-3d test"));
    assert (window);

    glb glb = {0};

    draw_buffer * draw_buffer = NULL;
    assert(draw_buffer_load_path(&draw_buffer, 1, argv + 1));
    assert (draw_buffer);

    vec_object3 origin = VEC_OBJECT3_INITIALIZER;

    draw_object object = { .origin = &origin };
    draw_object_set_mesh(&object, draw_buffer_index(draw_buffer, 0));

    draw_view view = { .quaternion = { .w = 1 }, .position.z = 5 };

    draw_shader_vertex * vert;

    assert (draw_shader_vertex_load_path(&vert, argv[2]));
    
    draw_shader_fragment * frag;

    assert (draw_shader_fragment_load_path(&frag, argv[3]));
    
    draw_shader_program * shader = NULL;
    assert (draw_shader_program_link(&shader, vert, frag));

    draw_shader_vertex_free(vert);
    draw_shader_fragment_free(frag);
    
    double start_time = 0;
    double delta_time;

    while (!ui_window_should_close(window))
    {
	draw_buffer_display (draw_buffer, &view, shader);
	//view.position.x = sin(start_time);
	delta_time = ui_get_time() - start_time;
	start_time += delta_time;
	fvec4_setup_rotation_quaternion(&origin.quaternion, &(fvec3){ .y = start_time });
	if (start_time == 0)
	{
	    continue;
	}
	ui_window_swap(window);
    }
    
    ui_window_destroy(window);
    draw_buffer_free(draw_buffer);
    
    gltf_clear(&glb.gltf);

    return 0;
}
