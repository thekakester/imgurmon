#ifndef GAME_C
#define GAME_C
#include <pebble.h>
#include "classes.h"  
  
//Also defined in game.h
#define NUM_IMGURMON 2
  
Imgurmon_Stats imgurmon_stats[NUM_IMGURMON];
  
  
/**Get an imgurmon with a given ID.  No error checking is done!
Be careful!
*/
Imgurmon_Stats* get_imgurmon_stats(int id) {
  return 0;//&(imgurmon_stats[id]);
}

//How to offset the screen
int xOffset;
int yOffset;

Entity player;
#endif