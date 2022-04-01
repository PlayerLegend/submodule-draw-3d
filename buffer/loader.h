#ifndef FLAT_INCLUDES
#include "def.h"
#endif

bool draw_buffer_load_batch (draw_buffer ** result, range_const_glb * input);

inline static bool draw_buffer_load_single (draw_buffer ** result, const glb * input)
{
    range_const_glb input_range = { .begin = input, .end = input + 1 };

    return draw_buffer_load_batch(result, &input_range);
}

void draw_buffer_free (draw_buffer * target);
