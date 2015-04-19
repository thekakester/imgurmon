#ifndef CLASSES_H
#define CLASSES_H 1
  #include <pebble.h>
  
    
typedef struct {
  char name[20];
  int hp;
  int type;
  int attack1;
  int attack2;
  int attack3;
  int evolve;
  int evolvelvl;
} Imgurmon_Stats;

//Imgurmon
typedef struct {
  Imgurmon_Stats stats;
  GBitmap* sprite;
  int hp;
} Imgurmon;

//Entity
typedef struct {
  float tween_percent;  //Number bewtween 0 and 1 (0%-100%)
  int x;
  int y;
  int newX;
  int newY;
  int direction;  //0,1,2,3 = down,left,right,up
} Entity;

char** map;        //Malloc'd in utility

int map_size;
GBitmap* tiles;
GBitmap* playerSprite;
Imgurmon imgurmon[2];
int buttonPressed;

#endif  
