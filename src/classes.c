#ifndef CLASSES_C
#define CLASSES_C 1
#include <pebble.h>
#include "includes.h"
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
#endif