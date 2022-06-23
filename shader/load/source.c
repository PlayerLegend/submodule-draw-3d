#include "source.h"
#include "../../../range/string.h"
#include "text.h"

bool draw_shader_vertex_load_source (draw_shader_vertex ** result, convert_source * input)
{
    convert_load_all (input);

    return draw_shader_vertex_load_text(result, input->contents->region.char_cast.begin);
}

bool draw_shader_fragment_load_source (draw_shader_fragment ** result, convert_source * input)
{
    convert_load_all (input);

    return draw_shader_fragment_load_text(result, input->contents->region.char_cast.begin);
}
