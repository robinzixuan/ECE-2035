#include "graphics.h"

#include "globals.h"
const char Man[121] = {
     'K','K','K','K','Y','Y','Y','Y','K','K','K',
    'K','K','K','Y','Y','Y','Y','Y','Y','K','K',
    'K','K','D','W','Y','Y','Y','Y','Y','K','K',
    'K','K','D','K','W','W','Y','W','W','K','K',
    'K','K','D','Y','K','Y','Y','Y','K','K','K',
    'K','K','D','D','Y','R','R','R','Y','K','K',
    'K','D','D','D','D','D','D','D','K','K','K',
    'K','D','Y','D','D','D','D','D','K','K','K',
    'K','D','Y','D','D','D','D','Y','K','K','K',
    'K','K','K','D','D','D','D','D','K','K','K',
    'K','K','K','Y','K','Y','K','D','K','K','K',
    };
    
const char oldMan[121] = {
    'B','B','B','B','S','S','S','S','B','B','B',
    'B','B','B','S','S','S','S','S','S','B','B',
    'B','B','3','W','S','S','S','S','S','B','B',
    'B','B','3','B','W','W','S','W','W','B','B',
    'B','B','3','S','B','S','S','S','B','B','B',
    'B','B','3','3','S','R','R','R','S','B','B',
    'B','D','D','D','3','3','3','3','B','B','B',
    'B','D','S','D','D','3','3','3','B','B','B',
    'B','D','S','D','D','D','3','S','B','B','B',
    'B','B','B','D','D','D','3','D','B','B','B',
    'B','B','B','S','B','S','B','D','B','B','B',
    };


void draw_player(int u, int v, int key)
{
    draw_img(u,v, Man);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}


void draw_upper_status()
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);
    
    // Add other status info drawing code here
}

void draw_lower_status()
{
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);
    
    // Add other status info drawing code here
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


const char duck[121] = {
    'B','B','B','B','B','B','Y','Y','Y','B','B',
    'B','B','B','B','B','Y','Y','Y','Y','Y','B',    
    'B','B','B','B','B','Y','Y','Y','B','Y','B',
    'B','B','B','B','B','Y','Y','Y','Y','O','R',
    'Y','B','Y','Y','Y','Y','Y','Y','Y','B','B',
    'Y','5','Y','Y','5','Y','Y','Y','Y','Y','B',
    'Y','Y','5','5','Y','Y','Y','Y','Y','Y','B',
    'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y',
    'B','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y',
    'B','Y','Y','Y','Y','Y','Y','Y','Y','Y','B',
    'B','Y','Y','Y','Y','Y','Y','Y','Y','Y','B',
    };
const char orange[121] = {
    'O','O','O','O','O','O','O','O','O','O','O',
    'O','O','O','G','G','O','D','O','O','O','O',
    'O','O','O','O','G','D','O','O','O','O','O',
    'O','O','O','O','O','O','O','O','O','O','O',
    'O','O','W','W','O','O','O','O','O','O','O',
    'O','O','W','O','O','O','O','O','O','O','O',
    'O','O','O','O','O','O','O','O','O','O','O',
    'O','O','O','O','O','O','O','O','O','O','O',
    'O','O','O','O','O','O','O','O','O','O','O',
    'O','O','O','O','O','O','O','O','O','O','O',
    };
    

    

void draw_key(int x, int y)
{
    uLCD.filled_circle(x, y, 10, YELLOW);
}


void draw_speaker(int x, int y)

{
    draw_img(x,y, oldMan);
}

void draw_attacker(int x, int y)
{
    draw_img(x,y, duck);
}

void draw_orange(int x, int y){
    draw_img(x,y, orange);
    }
 
 
const char plant [121] = {
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','G','B','G','B','G','G','B','B',
    'B','B','G','G','G','G','G','G','G','G','B',
    'B','G','G','G','G','G','G','G','G','G','B',
    'G','G','G','G','G','G','G','G','G','B','B',
    'B','G','G','G','G','G','G','G','G','G','B',
    'G','G','G','G','G','G','G','G','G','G','B',
    'G','G','G','G','G','G','G','G','G','G','G',
    };
    
void draw_plant(int u, int v)
{
    draw_img(u,v, plant);
}

const char plant_d [121] = {
   'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','B','B','B','B','B','B','B','B',
    'B','B','B','Y','B','Y','B','Y','Y','B','B',
    'B','B','Y','Y','Y','Y','Y','Y','Y','Y','B',
    'B','Y','Y','Y','Y','Y','Y','Y','Y','Y','B',
    'Y','Y','Y','Y','Y','Y','Y','Y','Y','B','B',
    'B','Y','Y','Y','Y','Y','Y','Y','Y','Y','B',
    'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','B',
    'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y',
    };
    
void draw_plant_dead(int u, int v)
{
    draw_img(u,v, plant_d);
}

void draw_door(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
}

void draw_stone(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}

void draw_big(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+200, v+200, BROWN);
}

void  draw_menu(){
    uLCD.cls();
    uLCD.locate(0,2);
    uLCD.printf("Status: press 2");
    uLCD.locate(2,2);
    uLCD.printf("Return: press 1");
    uLCD.locate(4,2);
    uLCD.printf("save");
    uLCD.locate(6,2);
    uLCD.printf("Instrction");
    }



void  draw_success(){
    uLCD.locate(0,2);
    uLCD.printf("You win");
    }


void  draw_fail(){
    uLCD.locate(0,2);
    uLCD.printf("You fail");
    }

