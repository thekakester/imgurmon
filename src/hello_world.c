#include <pebble.h>
#include "utility.h"
#include "renderer.h"
#include "classes.h"
#include "game.h"
  
#define ROTATE_THRESHOLD 0
  
Window* window;
Layer* layer;

void timer_handler(void* context) {
  layer_mark_dirty(layer);
  app_timer_register(34, timer_handler, NULL);
}

/**Gets called when we need to update our screen
*/
void update(Layer* layer, GContext* ctx) {
  render(ctx);  //Call our render in our renderer
}

void entity_create(Entity* entity) {
  entity->x = entity->newX = 32;
  entity->y = entity->newY = 60;
  entity->tween_percent = 0.0f;
  entity->direction = 0;  //Default looking down
}

/**Gets called when accelerometer info is given
Update: this basically turned into the movement handler.
All movement info is handled here, collision included
*/
static void data_handler(AccelData *data, uint32_t num_samples) {
  //Are we allowed to move yet?
  if (player.tween_percent >= 1 && mode == 0) {
    //Store old position
    player.x = player.newX;
    player.y = player.newY;
    
    //If we are out of a dead zone
    if (abs(data[0].x) > ROTATE_THRESHOLD || abs(data[0].y) > ROTATE_THRESHOLD) {
      
      //Decide where we should go
      //Up/Left/Down/Right, based on amount of tilt
      if (abs(data[0].x) > abs(data[0].y)) {
        if (data[0].x > 0) {
          player.newX++;      //Tilting to the right
        } else {
          player.newX--;      //Tilting to the left
        }
      } else {
        if (data[0].y > 0) {
          player.newY--;      //Tilting upward
        } else {
          player.newY++;      //Tilting downward
        }
      }
      
      //If there is a block where we are trying to go, just undo it
      if (isUnwalkable(map[player.newY][player.newX])) {
        //Hitting something!  Undo movement!
        player.newX = player.x;
        player.newY = player.y;
      }
      
      //If we're actually going to move, reset the tween
      if (player.newY != player.y || player.newX != player.x) {
        player.tween_percent = 0;
        
        //If there's a battle (Walking in tall grass)
        if (map[player.newY][player.newX] == 1) {
          srand (time(NULL));
          if (rand() % 5 == 0) {
            mode = 1;  //Switch to battle
            
          }
        }
        
        //Update our direction
        if (player.newY > player.y) { player.direction = 0;}  //Facing down
        if (player.newX < player.x) { player.direction = 1;}  //Facing left
        if (player.newX > player.x) { player.direction = 2;}  //Facing right
        if (player.newY < player.y) { player.direction = 3;}  //Facing up
      }
    }
  }
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
  int num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  
  //Load up our images
  tiles = gbitmap_create_with_resource(RESOURCE_ID_TILES);
  playerSprite = gbitmap_create_with_resource(RESOURCE_ID_PLAYER);
  
  //Load up the map
  load_map();
  
  //Load up all our imgurmon
  load_imgurmon();
  
  //Initialize our player object
  entity_create(&player);
  
  //Set a timer to call our update!
  timer_handler(NULL);
  
  //Set our imgurmon to null (so we know not to clear them)
  imgurmonSprite[0] = imgurmonSprite[1] = (GBitmap*)0;
  
  //Set our game mode to walking
  mode = 0;
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