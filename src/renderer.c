#include <pebble.h>
#include "classes.h" 
#include "utility.h"
#include "game.h"
  
  
void render_map(GContext* ctx) {
  for (int row = 0; row < map_size; row++) {
    for (int col = 0; col < map_size; col++) {
      int srcY = (map[row][col] / map_size) * 16;
      int srcX = (map[row][col] % map_size) * 16;
      graphics_draw_bitmap(ctx, tiles, GRect(srcX,srcY,16,16), GRect(col*16-xOffset,row*16+yOffset,16,16));
    }
  }
}  
  
//Render everything!
void render(GContext* ctx) {
  render_map(ctx);
}