#ifndef UTILITY_H
#define UTILITY_H 1
#include <pebble.h>
  
  void load_imgurmon();
  void loadImgurmon(int player, int id);
  void load_map();
  void graphics_draw_bitmap(GContext* ctx, GBitmap* bitmap, GRect src, GRect dest);
  int isUnwalkable(int tileType);
#endif