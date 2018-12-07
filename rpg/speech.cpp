#include "speech.h"

#include "globals.h"
#include "hardware.h"



int bi5=0;
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
     uLCD.filled_rectangle(0,100,127,127, BLACK);
}

void erase_speech_bubble()
{
    uLCD.filled_rectangle(0,100,127,127, WHITE);
}

void draw_speech_line(const char* line, int which)
{
    if(which == TOP)
    {
        uLCD.locate(1,13);
        uLCD.printf(line);
    }
    else if(which == BOTTOM)
    {
        uLCD.locate(1,14);
        uLCD.printf(line);
    }
}

int speech_bubble_wait(GameInputs inputs)
{
    if(inputs.b3 == 0 && bi5 == 0)
    {
        bi5 = 1;
        return 0;
    }
    else if(inputs.b3 == 1 && bi5 == 1)
    {
        bi5 = 0;
        return 1;
    }
    else
    {
        return 0;
    }    
}

void speech(const char* line1, const char* line2)
{
    int waiting = 1;
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    while(waiting == 1)
    {
        if(speech_bubble_wait(read_inputs()))
        {
            waiting = 0;
        }
    }
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{
}
