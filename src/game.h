#ifndef GAME_H
#define GAME_H 1
#include <pebble.h>
#include "classes.h"
#include "game.h"
  
//Prototypes
Imgurmon_Stats* get_imgurmon_stats(int id);

//How to offset the screen
int xOffset;
int yOffset;

Entity player;

#endif