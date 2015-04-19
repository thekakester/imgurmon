#ifndef UTILITY_C
#define UTILITY_C
#include <pebble.h>
#include <stdio.h>
#include "game.h"
#include "classes.h"
  
/*Create and store an imgurmon stats
Stores this to the specified imgurmon pointer
*/
void imgurmon_stats_create(Imgurmon_Stats* stats, char* name, int hp, int attack1, int attack2, int attack3, int type, int evolve, int evolvelvl) {
  strcpy(&(stats->name[0]), name);
  stats->hp = hp;
  stats->attack1 = attack1;
  stats->attack2 = attack2;
  stats->attack3 = attack3;
  stats->type = type;
  stats->evolve = evolve;
  stats->evolvelvl = evolvelvl;
}
  
//Turn a 4 byte array into an integer  
int buildInt(char* buffer) {
  int i = 0;
  int result = 0;
  for (i = 0; i < 4; i++) {
    result = result | (int)buffer[i];
    if (i < 3) {
      result = result << 1;
    }
  }
  //int result = *((int*)buffer); this doesnt work because of endiendness
  return result;
}  
  
/**All the initialization required to start the game.
This involves loading stats, the image, etc
*/
void loadStats(Imgurmon_Stats* stats, int id) {
  char s_buffer[28];
  // Get resource
  ResHandle handle = resource_get_handle(RESOURCE_ID_STATS);
  //resource_load(handle, (uint8_t*)s_buffer, res_size);
  
  //Each imgurmon takes up 28 bytes, so start at 28*id, and read 28 bytes
  resource_load_byte_range(handle, 28*id, (uint8_t*)&(s_buffer[0]), 28);
  
  //Read in the imgurmon
  //An imgurmon takes up 28 bytes
    /*Our input file is structured like such:
				 * 20 bytes - Name (including null terminator)
				 * 2  bytes - HP (0-65535)
				 * 1  byte  - Attack1 id
				 * 1  byte  - Attack2 id
				 * 1  byte  - Attack3 id
				 * 1  byte  - type
				 * 1  byte  - ID of evolution
				 * 1  byte  - Evolution Level
				 * 
				 * Total Size: 28bytes per entry!
				 */
    //Sbuffer is the name
    strncpy(stats->name,s_buffer,19);
  
    //Bytes 20 and 21 are hp.  Turn this into a buffer
    char buffer[4] = {0,0,s_buffer[20],s_buffer[21]};
    stats->hp        = buildInt(buffer);
    stats->attack1   = (int)s_buffer[22];
    stats->attack2   = (int)s_buffer[23];
    stats->attack3   = (int)s_buffer[24];
    stats->type      = (int)s_buffer[25];
    stats->evolve    = (int)s_buffer[26];
    stats->evolvelvl = (int)s_buffer[27];
    
    printf("Loading %s. HP:%d, attacks:%d:%d:%d, type:%d, evolvesto:%d, evolvelvl:%d\n",stats->name,stats->hp,stats->attack1,stats->attack2,stats->attack3,stats->type,stats->evolve,stats->evolvelvl);
    //Store this imgurmon
    //imgurmon_stats_create(&(imgurmon_stats[i]),substring,hp,attack1,attack2,attack3,type,evolve,evolvelvl);
}

void load_map() {
  //Read the map file, and look at the first integer to get the size
  char* s_buffer;
  
  // Get resource and size
  ResHandle handle = resource_get_handle(RESOURCE_ID_MAP);
  int res_size = resource_size(handle);

  // Copy to buffer
  s_buffer = (char*)malloc(res_size);
  resource_load(handle, (uint8_t*)s_buffer, res_size);
  
  //Now that the map is loaded, lets start copying stuff!
  map_size = buildInt(s_buffer);
  
  char* data = &(s_buffer[4]);
  int row,col;
  
  //Allocate the rows
  map = (char**)malloc(sizeof(char*)*map_size);

  int index = 0;  //Where are we in the file?
  
  //Begin reading in the map file
  for (row = 0; row < map_size; row++) {
    //Allocate this column
    map[row] = (char*)malloc(sizeof(char)*map_size);
    
    //Read the map data and apply it
    for (col = 0; col < map_size; col++) {
      //Read in the map file and store it here!
      map[row][col] = data[index];
      index++;
    }
  }
  
  //Free the map buffer
  free(s_buffer);
}

/**Simple drawing method that lets us take advantage of spritesheets
GRect src is the source rectangle (where to pull the image from) and
GRect dest is where to draw it on the screen
Note, this does not do any scaling
*/
void graphics_draw_bitmap(GContext* ctx, GBitmap* bitmap, GRect src, GRect dest) {
  GBitmap* sprite = gbitmap_create_as_sub_bitmap(bitmap, src);
  graphics_draw_bitmap_in_rect(ctx, sprite, dest);
  free(sprite);
}

/*Get the resource ID given an imgurmon ID.
Resource IDs are not consistent, and tend to get shuffled during compile.
This is the best way to handle it!
*/
int getResourceID(int id) {
  switch(id) {
    case 0:  return RESOURCE_ID_IMGURMON0;
    case 1:  return RESOURCE_ID_IMGURMON1;
    case 2:  return RESOURCE_ID_IMGURMON2;
    case 3:  return RESOURCE_ID_IMGURMON3;
    case 4:  return RESOURCE_ID_IMGURMON4;
    case 5:  return RESOURCE_ID_IMGURMON5;
    case 6:  return RESOURCE_ID_IMGURMON6;
    case 7:  return RESOURCE_ID_IMGURMON7;
    case 8:  return RESOURCE_ID_IMGURMON8;
    case 9:  return RESOURCE_ID_IMGURMON9;
    case 10:  return RESOURCE_ID_IMGURMON10;
    case 11:  return RESOURCE_ID_IMGURMON11;
    case 12:  return RESOURCE_ID_IMGURMON12;
    case 13:  return RESOURCE_ID_IMGURMON13;
    case 14:  return RESOURCE_ID_IMGURMON14;
    default: return RESOURCE_ID_IMGURMON0;
  }
}

/**There are 2 slots for loading imgurmon, one is yours,
and the other belongs to your enemy.  This will free any previously malloc'd
resource, and re-load the image
player = 0 or 1, the index where to store
id = 0-#imgurmon.  The id of the imgurmon to load
*/
void loadImgurmon(int player, int id) {
  if (imgurmon[player].sprite) {
    //We need to avoid a memory leak
    gbitmap_destroy(imgurmon[player].sprite);
  }
  
  //Load up the sprite
  imgurmon[player].sprite = gbitmap_create_with_resource(getResourceID(id));
  
  loadStats(&(imgurmon[player].stats),id);
  
  //Set the HP and stuff
  imgurmon[player].hp = imgurmon[player].stats.hp;
}

/**The "64" bit determines whether or not a tile is walkable
*/
int isUnwalkable(int tileType) {
  //If this evaluates to anything other than 0, there is collision
  return (tileType & 0x40);
}

#endif