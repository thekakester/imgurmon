#include <pebble.h>
#include "utility.h"
  
Window* window;
Layer* layer;
GBitmap* tiles;
int x = 0;
int y = 0;

void timer_handler(void* context) {
  layer_mark_dirty(layer);
  app_timer_register(34, timer_handler, NULL);
}

void update(Layer* layer, GContext* ctx) {
  x = (x+1)%(256);
  if (x % 10 == 0) { y = (y+1)%256; }
  
  graphics_draw_bitmap(ctx,tiles,GRect(x,y,144,169),GRect(0,0,144,169));
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  window_stack_push(window,false);
  layer = window_get_root_layer(window);
	layer_set_update_proc(layer,update);
  layer_mark_dirty(layer);
  
  //Load up our tileset
  tiles = gbitmap_create_with_resource(RESOURCE_ID_TILES);
  timer_handler(NULL);
  
  //Load up all our imgurmon
  load_imgurmon();
}

void handle_deinit(void) {
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}