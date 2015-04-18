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
  
  
typedef struct {
  char name[20];
  int hp;
  Type type;
  int attack1;
  int attack2;
  int attack3;
  int evolve;
  int evolvelvl;
} Imgurmon_Stats;

char** map;
char** collision;
GBitmap* tiles;
GBitmap* playerSprite;
int map_size;



#endif