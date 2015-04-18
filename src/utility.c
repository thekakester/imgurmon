#include <pebble.h>
#include <stdio.h>  
  
/**All the initialization required to start the game.
This involves loading stats, the image, etc
*/
void load_imgurmon() {
  
}

/**Simple drawing method that lets us take advantage of spritesheets
GRect src is the source rectangle (where to pull the image from) and
GRect dest is where to draw it on the screen
Note, this does not do any scaling
*/
void graphics_draw_bitmap(GContext* ctx, GBitmap* bitmap, GRect src, GRect dest) {
  GBitmap* sprite = gbitmap_create_as_sub_bitmap(bitmap, src);
  graphics_draw_bitmap_in_rect(ctx, sprite, dest);
  free(sprite);
}