#ifndef CLASSES_C
#define CLASSES_C 1
#define MAP_SIZE 16;

#include <pebble.h>
//Types
#define Type int
#define NONE 0
#define FIRE 1
#define WATER 2
#define GRASS 3


char** map;
GBitmap* tiles;
GBitmap* playerSprite;
int map_size;
GBitmap* imgurmonSprites[2];



#endif