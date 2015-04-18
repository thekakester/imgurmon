#include <pebble.h>
#include "utility.h"
#include "renderer.h"
#include "classes.h"
#include "game.h"
  
Window* window;
Layer* layer;
int x = 0;
int y = 0;

void timer_handler(void* context) {
  layer_mark_dirty(layer);
  app_timer_register(34, timer_handler, NULL);
}

/**Gets called when we need to update our screen
*/
void update(Layer* layer, GContext* ctx) {
  render(ctx);  //Call our render in our renderer
}

/**Gets called when accelerometer info is given
*/
static void data_handler(AccelData *data, uint32_t num_samples) {
  xOffset += data[0].x / 40;
  yOffset += data[0].y / 40;
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  window_stack_push(window,false);
  layer = window_get_root_layer(window);
  
  //Set our update procedure (what gets called when we refresh the screen)
	layer_set_update_proc(layer,update);
  layer_mark_dirty(layer);
  
  //Set our accelerometer procedure
  int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);
  
  //Load up our tileset
  tiles = gbitmap_create_with_resource(RESOURCE_ID_TILES);
  timer_handler(NULL);
  
  //Load up all our imgurmon
  load_imgurmon();
  
  //Load up the map
  load_map();
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