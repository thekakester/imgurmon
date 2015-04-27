#ifndef CLASSES_H
#define CLASSES_H 1
#include <pebble.h>
  
#define BUTTON_NO_PRESS 0
#define BUTTON_UP 1
#define BUTTON_SELECT 2
#define BUTTON_DOWN 3
#define BUTTON_BACK 4
  
  
    
typedef struct {
  char name[20];
  int hp;
  int attacks[3];      //Array of attack IDS
  int type;
  int evolve;
  int evolvelvl;
} Imgurmon_Stats;

//Attacks
typedef struct {
  char name[15];
  int type;            //Type of attack.  Eg, fire, water, grass, none
  int low;             //The least damage this can do
  int high;            //The most damage this can do
  int count;           //How many attacks can you do max?
  char success_rate;   //0-255.  0 = 0% success rate, 255 = 100% success rate (no miss)
} Attack;

//Imgurmon
typedef struct {
  Imgurmon_Stats stats;
  GBitmap* sprite;
  Attack attacks[3];  //The attacks that this imgurmon has
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

/**Battle stuff**/
char menu;          //255 means no menu.  0,1,2,3 is selected item

#endif  
