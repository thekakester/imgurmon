#ifndef GAME_H
#define GAME_H 1
#include <pebble.h>
#include "classes.h"
#include "game.h"

//Also defined in game.c
#define NUM_IMGURMON 40
  
//Prototypes
Imgurmon_Stats* get_imgurmon_stats(int id);

//How to offset the screen
int xOffset;
int yOffset;

//What is our game mode?  0 = normal, 1 = battle
int mode;

Entity player;

#endif