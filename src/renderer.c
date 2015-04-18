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
      int srcY = (map[row][col] / map_size) * 16;
      int srcX = (map[row][col] % map_size) * 16;
      
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
  
  graphics_draw_bitmap(ctx, playerSprite, GRect(16,0,16,16), GRect(HALFWIDTH,HALFHEIGHT,16,16));
}
  
//Render everything!
void render(GContext* ctx) {
  render_map(ctx);
  render_player(ctx);
}