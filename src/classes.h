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
  int maxHp;
  int exp;
  Type type;
} Imgurmon;