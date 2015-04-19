#include <pebble.h>
#include "classes.h" 
#include "utility.h"
#include "game.h"
  
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
  
#define HALFWIDTH 72
#define HALFHEIGHT 84
  
float tween(float x, float xi, float tween) {
  if (x == xi) { return x; }  //Dont calculate!  Avoid loss of precsion!
  return (x * (1.0f-tween)) + (xi * tween);
}
  
void render_map(GContext* ctx) {
  //This is also the player's position
  xOffset = (int)(tween(player.x,player.newX,player.tween_percent)*16) - HALFWIDTH;
  yOffset = (int)(tween(player.y,player.newY,player.tween_percent)*16) - HALFHEIGHT;
  
  //We can see 6 tiles to the left of us, 6 tiles to the right, 7 above and 7 below
  //Only render these tiles
  for (int row = MAX(0,player.y-7); row < MIN(map_size,player.y+7); row++) {
    for (int col = MAX(0,player.x-6); col < MIN(map_size,player.x+6); col++) {
      //Remove collision bit if there is one
      int type = (map[row][col] >= 64) ? map[row][col] - 64 : map[row][col];
      int srcY = (type / 8) * 16;
      int srcX = (type % 8) * 16;
      
      graphics_draw_bitmap(ctx, tiles, GRect(srcX,srcY,16,16), GRect(col*16-xOffset,row*16-yOffset,16,16));      
    }
  }
}  

//Render the player
void render_player(GContext* ctx) {
  
  //Update the player's tween
  player.tween_percent += 0.1f;
  if (player.tween_percent > 1) { player.tween_percent = 1;}
  
  //In draw map, we calculated xOffset and yOffset.
  //If we did PlayerPos - offset, it would be the same as (HalfWidth,HalfHeight)
  //with basic algebra
  
  //When walking, go left, center, right, center
  int frame = (int)(player.tween_percent * 3);
  if (frame == 3) { frame = 1; } //Center
  graphics_draw_bitmap(ctx, playerSprite, GRect(frame*16,player.direction*16,16,16), GRect(HALFWIDTH,HALFHEIGHT,16,16));
}
  
//For rendering loading frame.  A useless feature
int renderFrame = 0;

//Render everything!
void render(GContext* ctx) {
  //Set screen black for first render
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0,0,144,169), 0, GCornerNone);
  
  if (mode > 0) {
    mode = (mode == 1) ? 2 : 1;  //Toggle between 1 and 2
    if (mode == 1) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_fill_rect(ctx, GRect(0,0,144,169), 0, GCornerNone);
      return;
    }
  }
  
  
  render_map(ctx);
  render_player(ctx);
}