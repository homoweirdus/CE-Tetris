// Converted using ConvPNG
// This file contains all the graphics sources for easier inclusion in a project
#ifndef __tiles_gfx__
#define __tiles_gfx__
#include <stdint.h>

#define tiles_gfx_transparent_color_index 0

extern uint8_t set_tile_0_data[];
#define set_tile_0 ((gfx_sprite_t*)set_tile_0_data)
extern uint8_t set_tile_1_data[];
#define set_tile_1 ((gfx_sprite_t*)set_tile_1_data)
extern uint8_t set_tile_2_data[];
#define set_tile_2 ((gfx_sprite_t*)set_tile_2_data)
extern uint8_t set_tile_3_data[];
#define set_tile_3 ((gfx_sprite_t*)set_tile_3_data)
extern uint8_t set_tile_4_data[];
#define set_tile_4 ((gfx_sprite_t*)set_tile_4_data)
extern uint8_t set_tile_5_data[];
#define set_tile_5 ((gfx_sprite_t*)set_tile_5_data)
#define set_tiles_num 6
extern uint8_t *set_tiles_data[6];
#define set_tiles ((gfx_sprite_t**)set_tiles_data)
#define sizeof_tiles_gfx_pal 14
extern uint16_t tiles_gfx_pal[7];

#endif
