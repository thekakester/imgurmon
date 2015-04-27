#include <pebble.h>
#include "classes.h" 
#include "utility.h"
#include "game.h"
  
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
  
#define HALFWIDTH 72
#define HALFHEIGHT 84
  
float tween(float x, float xi, float tween) {
  if (x == xi) { return x; }  //Dont calculate!  Avoid loss of precsion!
  return (x * (1.0f-tween)) + (xi * tween);
}


//A variable for keeping track of ticks 
int tickCounter = 0;   //Random counter.  Used for tweening usually
int introCounter = 0;  //Used for moving imgurmon in to battle!


int random(int low, int high) {
  int range = high - low;
  return (rand() % range) + low;
}

/*Draw our and our opponents imgurmon
Note: This also handles attacks and whatnot, so it might get complicated!*/
void drawImgurmon(GContext* ctx) {
  
  //Increase the introCounter until we are already in the game
  if (introCounter < 144) {
    introCounter += 3;
    
    //This gets called when we finish
    if (introCounter >= 144) {
      introCounter = 144;  //Dont go past 144
      mode++;              //Move on to select attack mode(3)!
    }
  }
  
  int yChangeEnemy = 0;  //How much to offset the y position by (use for attacking)
  if (mode == 8) {
    //first 25 frames are up, next are down
    if (tickCounter / 25 == 0) { yChangeEnemy = tickCounter; }
    else { yChangeEnemy = 50 - tickCounter; }
  }
  
  int yChangeUs = 0;  //How much to offset the y position by (use for attacking)
  if (mode == 4) {
    //first 25 frames are up, next are down
    if (tickCounter / 25 == 0) { yChangeUs = tickCounter; }
    else { yChangeUs = 50 - tickCounter; }
  }
  
  //Move each imgurmon in for each turn (this to left)
  int x = introCounter - 57;
  //If we are in attack mode, flash him
  if (mode == 5 && tickCounter % 2) {
    //Do nothing.  (don't draw)
  } else {
    graphics_draw_bitmap_in_rect(ctx, imgurmon[1].sprite, GRect(x, 0+yChangeEnemy, 56, 56));
  }
  
  //Move our imgurmon to the right
  x = 144 - introCounter;
  //Flash if we are in attack mode!
  if (mode == 9 && tickCounter % 2) {
    //Do Nothing (don't draw)
  } else {
    graphics_draw_bitmap_in_rect(ctx, imgurmon[0].sprite, GRect(x, 100-yChangeUs, 56, 56));
  }
}

/*Draw the menu during a battle!
This got enough code in it that I thought it would be best in its own menu
*/
void drawMenu (GContext* ctx) { 
  
  //Hide menu!
  if (buttonPressed == BUTTON_BACK) {
    menu = 255;
    buttonPressed = BUTTON_NO_PRESS;
  }
  
  //If they pressed a button and there's no menu, show it!
  if (menu == 255 && buttonPressed) {
    menu = 0;
    buttonPressed = BUTTON_NO_PRESS;  //Dont allow duplicates!
  }
  
  //If the menu is active...
  if (menu != 255) {
    //Allow them to move up and down
    if (buttonPressed == BUTTON_UP) {
      menu = (menu + 2) % 3;    //3 items, add 2, mod 3 to go back one
    }
    if (buttonPressed == BUTTON_DOWN) {
      menu = (menu + 1) % 3;    //3 items, add 1, mod 3 to go forward one
    }
    if (buttonPressed == BUTTON_SELECT) {
      //Select this as our attack!
      chosen_attack = menu;
      menu = 255;            //Hide the menu now!
      mode++;                //Move into P1 Attack mode (3)
    }
    
    //DRAW THE MENU!
    graphics_context_set_fill_color(ctx,GColorWhite);
    graphics_fill_rect(ctx, GRect(0,100,144,69), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(1,101,142,67));
    
    //Draw the attacks that the player has
    for (int i = 0; i < 3; i++) {
      if (menu == i) {
        graphics_draw_rect(ctx,GRect(2,112+(i*16),4,2));
      }
      graphics_draw_text(ctx, imgurmon[0].attacks[i].name, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(10,102 + (i*16),144,169), GTextOverflowModeFill, GTextAlignmentLeft, NULL);
    }
  }
}

void battle(GContext* ctx) {
  
  /**Before we get into actual code, let me outline the different modes
  0 - Not in battle
  1 - Intro to battle (Screen flashing)
  2 - Intro to battle2 (imgurmon move in from sides)
  3 - Select attack (use menu to pick attack)
  4 - We attack (our imgurmon "moves" to attack)
  5 - Opponent Hit (opponent is hit by the attack)
  6 - Opponent HP Drop (opponent loses HP)       **REMOVED**
  7 - Opponent selecting attack (intended for future multiplayer)
  8 - Opponent Attack (Enemy moves to attack)
  9 - We are hit (we flash to show we are hit)
  10 - We lose HP (Our HP Decreases from attack) **REMOVED**
  11 - End
  */
  
  //Clear the background
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(0,0,144,169), 0, GCornerNone);
 
  //If this is our first frame in here, select a pokemon to battle!
  //Also, load everything up!
  if (tickCounter == 0) {
    //Select random imgurmon
    int id = startingImgurmon;
    loadImgurmon(0,id);
    id = rand() % NUM_IMGURMON;
    loadImgurmon(1,id);
    
    introCounter = 0;
    menu = 255;  //Make sure we don't show the menu
    
    tickCounter++;  //Make sure we dont do this again!
  }
  
  //Draw us and our opponent
  drawImgurmon(ctx);
  
  //Draw the HP bars
  if (mode >= 3) {
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_text_color(ctx, GColorBlack);
    
    graphics_draw_rect(ctx, GRect(2,2,87,32));
    graphics_draw_rect(ctx, GRect(4,24,83,6));  //Width of 83
    float percent = (float)imgurmon[1].hp / (float)imgurmon[1].stats.hp;
    graphics_draw_rect(ctx, GRect(6,26,(int)(79*percent),2));
    graphics_draw_text(ctx, imgurmon[1].stats.name, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(4,4,83,20), GTextOverflowModeFill, GTextAlignmentLeft, NULL);
    
    graphics_draw_rect(ctx, GRect(57,100,87,32));
    graphics_draw_rect(ctx, GRect(59,124,83,6));
    percent = (float)imgurmon[0].hp / (float)imgurmon[0].stats.hp;
    graphics_draw_rect(ctx, GRect(61,126,(int)(79*percent),2));
    graphics_draw_text(ctx, imgurmon[0].stats.name, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(59,102,83,20), GTextOverflowModeFill, GTextAlignmentLeft, NULL);
    
  }
  
  //Skip some removed mechanics
  if (mode==6) {
    mode++;  //No longer have HP tween stage
    
    //Exit if we won
    if (imgurmon[1].hp == 0) { mode = 0; }
  }
  
  //TWEEN STATE!
  //We/opponent are attacking
  //We/opponent are taking damage
  if (mode >= 4) {
    tickCounter++;
    if (tickCounter > 50) {
      tickCounter = 1;  //NOT ZERO!  Otherwise we would re-load the match!
      mode++;           //Let us attack again!
      if (mode > 9) { mode = 3; }  //Back to our turn
    }
    
    //Exit if someone died
    if (imgurmon[0].hp == 0) { mode = 0; }
  }
  
  if (mode == 3) {
    drawMenu(ctx);
  }
  
  //Apply damage and such!
  int attacker = -1;
  if (tickCounter == 1) {
    if (mode == 5) { attacker = 0; }
    if (mode == 9) { attacker = 1; }
  }
  
  //If someone is actually dealing damage!
  if (attacker >= 0) {
    int receiver = (attacker + 1) %2;
    
    //Apply damage
    Attack attack = imgurmon[attacker].attacks[chosen_attack];
    int damage = random(attack.high,attack.low);
    
    //Apply this to the other player
    imgurmon[receiver].hp -= damage;
    if (imgurmon[receiver].hp < 0) { imgurmon[receiver].hp = 0; }
  }
}
  
void render_map(GContext* ctx) {
  //This is also the player's position
  xOffset = (int)(tween(player.x,player.newX,player.tween_percent)*16) - HALFWIDTH;
  yOffset = (int)(tween(player.y,player.newY,player.tween_percent)*16) - HALFHEIGHT;
  
  //We can see 6 tiles to the left of us, 6 tiles to the right, 7 above and 7 below
  //Only render these tiles
  for (int row = MAX(0,player.y-7); row < MIN(map_size,player.y+7); row++) {
    for (int col = MAX(0,player.x-6); col < MIN(map_size,player.x+6); col++) {
      //Remove collision bit if there is one
      int type = (map[row][col] >= 64) ? map[row][col] - 64 : map[row][col];
      int srcY = (type / 8) * 16;
      int srcX = (type % 8) * 16;
      
      graphics_draw_bitmap(ctx, tiles, GRect(srcX,srcY,16,16), GRect(col*16-xOffset,row*16-yOffset,16,16));      
    }
  }
}  

//Render the player
void render_player(GContext* ctx) {
  
  //Update the player's tween
  player.tween_percent += 0.1f;
  if (player.tween_percent > 1) { player.tween_percent = 1;}
  
  //In draw map, we calculated xOffset and yOffset.
  //If we did PlayerPos - offset, it would be the same as (HalfWidth,HalfHeight)
  //with basic algebra
  
  //When walking, go left, center, right, center
  int frame = (int)(player.tween_percent * 3);
  if (frame == 3) { frame = 1; } //Center
  graphics_draw_bitmap(ctx, playerSprite, GRect(frame*16,player.direction*16,16,16), GRect(HALFWIDTH,HALFHEIGHT,16,16));
}

//Render everything!
void render(GContext* ctx) {
  //Set screen black for first render
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0,0,144,169), 0, GCornerNone);
  
  if (mode == 1) {
    //Transitioning into battle
    //Increase our counter, flash our screen, and eventually stop
    tickCounter++;
    if ((tickCounter / 2)%2 == 0) { return; }  //Stop drawing(black screen effect)
    if (tickCounter > 50) {
      //Transition into battle
      tickCounter = 0;   //Reset our counter
      mode++;            //Battle mode (2)
    }
  } else if (mode > 1) {
    //We're in battle!
    battle(ctx);
    return;
  }
  
  
  render_map(ctx);
  render_player(ctx);
}