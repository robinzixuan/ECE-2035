// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include "SDFileSystem.h"
#include "mbed.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map1 ();
void init_main_map2 ();
int main ();
void save();
void load();
void music(); 
int start();
void check_status();
int Button(GameInputs inputs);
int bi1 = 0;
int bi2 = 0;
int bi3 = 0;
int bi4 = 0;
int a=0;
int c=0;
int b=0;
/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int health;
    int Orange;
    int attack;
    int task;
    int end;
    int omi;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define Omnipotent 7
#define NPC1 9
#define  ATTACKER 10
#define PLANT 11
#define DOOR 12
#define stone 13
#define NPC2 15


void save(){
   // sd.write();
    FILE *fp = fopen("/sd/myfile.txt", "w");
    fprintf(fp, "X:%d\n", Player.x);
    fprintf(fp, "Y:%d\n", Player.y);
    fprintf(fp, "x:%d\n", Player.px);
    fprintf(fp, "y:%d\n", Player.py);
    fprintf(fp, "H:%d\n", Player.health);
    fprintf(fp, "T:%d\n", Player.task);
    fprintf(fp, "O:%d\n", Player.Orange);
    fprintf(fp, "E:%d\n", Player.end);
    fprintf(fp, "K:%d\n", Player.has_key);
    fprintf(fp, "A:%d\n", Player.attack);
    MapItem* A1=get_here(8,10);
    MapItem* A2=get_here(18,30);
    fprintf(fp, "%d\n", A1->attack);
    fprintf(fp, "%d\n", A2->attack);
    fclose(fp);
    }
    
int get_action(GameInputs inputs)
{
    int i= Button(inputs);
    MapItem* N=get_north(Player.x,Player. y);
    MapItem* S=get_south(Player.x,Player.y);
    MapItem* W=get_east(Player.x, Player.y);
    MapItem* E=get_west( Player.x, Player.y);
    if ( N->type==NPC1 || S->type==NPC1 ||  W->type==NPC1 ||  E->type==NPC1)
        return NPC1;
    if ( N->type==NPC2|| S->type==NPC2 ||  W->type==NPC2 ||  E->type==NPC2)
        return NPC2;
    if  ( N->type==ATTACKER || S->type==ATTACKER ||  W->type==ATTACKER ||  E->type==ATTACKER)
        return ATTACKER;
    if  ( N->type==stone || S->type==stone ||  W->type==stone ||  E->type==stone)
        return stone;
    if  ( N->type==DOOR || S->type==DOOR ||  W->type==DOOR ||  E->type==DOOR)
        return DOOR;
    if  ( N->type==PLANT || S->type==PLANT ||  W->type==PLANT ||  E->type==PLANT)
        return PLANT;
    if ((W->walkable==1 || Player.omi==1 || (W==NULL && Player.px>1)) && (inputs.ax <= -0.5) )
        return GO_LEFT;
    if ((E->walkable==1 || Player.omi==1 || (E==NULL && Player.px<49)) && (inputs.ax >= 0.5)  )
        return GO_RIGHT;
    if ((N->walkable==1 || Player.omi==1 || (N==NULL && Player.py>1)) && (inputs.ay <= -0.5)  )
        return GO_UP;
    if ((S->walkable==1 || Player.omi==1 || (S==NULL && Player.py<49)) && (inputs.ay >= 0.5) )
        return GO_DOWN;
    if (i==4)
        return ACTION_BUTTON;
    if (i==2)
        return Omnipotent;
    if (i==1)
        return MENU_BUTTON;
    else 
        return NO_ACTION;
}


int Button(GameInputs inputs){
    if(inputs.b1 == 0 && bi1 == 0)
         bi1 = 1;
    if(inputs.b1 == 1 && bi1 ==1){
        bi1=0;
        return 1;
        }
    if(inputs.b2 == 0 && bi2 == 0)
         bi2 = 1;
    if(inputs.b2 == 1 && bi2 ==1){
        bi2=0;
        return 2;
        }
    if(inputs.b3 == 0 && bi3 == 0)
         bi3 = 1;
    if(inputs.b3 == 1 && bi3 ==1){
        bi3=0;
        return 3;
        }
    if(inputs.b4 == 0 && bi4 == 0)
         bi4 = 1;
    if(inputs.b4 == 1 && bi4 ==1){
        bi4=0;
        return 4;
        } 
    return 0;     
    }

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:     
            Player.y = Player.y + 1; 
            break;
        case GO_LEFT:   
            Player.x = Player.x - 1; 
            break;           
        case GO_DOWN:   
            Player.y = Player.y - 1; 
            break;
        case GO_RIGHT:  
            Player.x = Player.x + 1; 
            break;
        case NPC1:
            if (Player.task==0 && Player.health < 75)
            {
                speech("Hello, warrior!","You are hurt.");
                speech("show you some orange ","it can increase your health");
                speech("Pleace finish your task for the attack","Bye!");
                add_orange(21,16);
                draw_orange(231,176);
                add_orange(21,17);
                draw_orange(231,187);
                add_orange(21,18);
                draw_orange(231,198);
                Player.y = Player.y - 1; 
            }
             else if (Player.task==1 ){
                 speech("Hello, warrior!", "you are good!");
                 speech("Here is key", "you can use it open door");
                 add_key(31,16);
                 draw_key(341,176);
                 Player.y = Player.y - 1; 
            }
             else{
                speech("Come on, warrior!", "Pleace finish your task for the attack");    
                speech(".......","........");
                Player.y = Player.y - 1;
             }
            break;
        case NPC2:
            if (Player.task==0)
            {
                speech("Hello, warrior!","Place find and move stone.");
                Player.task=1;
                Player.y = Player.y - 1; 
                }
            else{
                 speech("Hello, warrior!", "you are good!");
                 speech("Here is key", "you can use it open door");
                 add_key(31,16);
                 draw_key(341,176);
                 Player.y = Player.y - 1; 
                 }
            break;
        case ATTACKER:
            if (b<2){
            speech("Come on", "fight with it");
            b+=1;
            deleteHere(Player.x,Player.y);
            Player.y = Player.y - 1; 
            }
            else{
                Player.task=1;
                speech("Congratulation", "you win");
                }
            break;
        case stone:
            int i= Button(read_inputs());
            MapItem* N=get_north(Player.x,Player. y);
            MapItem* S=get_south(Player.x,Player.y);
            MapItem* W=get_east(Player.x, Player.y);
            MapItem* E=get_west( Player.x, Player.y);
            if (W->type==stone){
                deleteHere(Player.x-1,Player.y);
                uLCD.filled_rectangle(11*(Player.x-1),11*Player.y,11*(Player.x),11*Player.y, WHITE);
                add_stone(Player.x-2,Player.y);
                draw_stone(Player.x-2,Player.y);
                Player.y=Player.y-1;
                }
            if (E->type==stone){
                deleteHere(Player.x+1,Player.y);
                 uLCD.filled_rectangle(11*(Player.x+1),11*Player.y,11*Player.x,11*Player.y, WHITE);
                add_stone(Player.x+2,Player.y);
                 draw_stone(Player.x+2,Player.y);
                 Player.y=Player.y-1;
                }
            if (N->type==stone){
                deleteHere(Player.x,Player.y+1);
                 uLCD.filled_rectangle(11*Player.x,11*(Player.y+1),11*Player.x,11*Player.y, WHITE);
                add_stone(Player.x,Player.y+2);
                 draw_stone(Player.x,Player.y+2);
                 Player.y=Player.y-1;
                }
            if (S->type==stone){
                deleteHere(Player.x,Player.y-1);
                uLCD.filled_rectangle(11*Player.x,11*(Player.y-1),11*Player.x,11*Player.y, WHITE);
                add_stone(Player.x,Player.y-2);
                draw_stone(Player.x,Player.y-2);
                Player.y=Player.y-1;
                }
            break;
        case DOOR:
            if (Player.has_key==1){
                speech("You open the door", "welcome back home");
                add_door_o(20,10);
                Player.has_key=0;
                Player.end=1;
                Player.y = Player.y - 1; 
            }else{
                speech("The door is locked","Continue your task.");
                Player.y = Player.y - 1; 
                }
            break;
        case PLANT:
            speech("You get 2 oranges", "You eat it");
            Player.health+=40;
            deleteHere(10,5);
            add_plant_dead(10,5);
            draw_plant_dead(110,55);
            Player.y = Player.y - 1; 
            break;
        case ACTION_BUTTON: 
            MapItem* Here=get_here( Player.x, Player.y);
            Player.health-=20;
            Here->attack-=50;
            if (Here->attack<=0){
                b+=1;
                deleteHere(Player.x,Player.y);
                }
            break;
        case MENU_BUTTON: 
            c=1;
            uLCD.filled_rectangle(0,0,550,550, WHITE);
            draw_menu();
            int j= Button(read_inputs());
            switch (j)
            {
                case 1:
                    if (a==1){
                         init_main_map1();
                         c=0;
                         }
                    else if (a==2){
                        init_main_map2();
                        c=0;
                        }
                case 2:
                    uLCD.filled_rectangle(0,0,550,550, WHITE);
                    uLCD.locate(0,1);
                    uLCD.printf("Status:");
                     uLCD.locate(0,2);
                     uLCD.printf("X:%d", Player.x);
                     uLCD.locate(4,2);
                     uLCD.printf("Y:%d", Player.y);
                     uLCD.locate(0,3);
                     uLCD.printf("Health:%d", Player.health);
                     uLCD.locate(0,4);
                     uLCD.printf("Orange:%d", Player.Orange);
                case 3:
                    save();
                    break;
                case 4:
                    uLCD.filled_rectangle(0,0,550,550, WHITE);
                    uLCD.locate(9,15);
                    uLCD.printf("You should finish the task.");
                    uLCD.locate(10,15);
                    uLCD.printf("You should kill all the attacker");
                    uLCD.locate(11,15);
                    uLCD.printf("Eash attacker can attack you one turn for 20 hurt and you get attack him 50 hurts");
                    uLCD.locate(12,15);
                    uLCD.printf("You have 150 life and each attacker have 100 life, and you can eat orange to add 20 life per time");
                    uLCD.locate(12,15);
                    uLCD.printf("After you finish, you can go to NPC to get one key to open the door, then go back home.");
                    break;
                default:
                    break;
                }
            break;
        case Omnipotent:
            Player.omi=1;
            break;
        default:        
            break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map1()
{
    uLCD.filled_rectangle(0,0,550,550, WHITE);
    Map* map = set_active_map(0);
    add_wall(0,              0,              HORIZONTAL, map_width());
    for(int i = 0; i < map_width(); i++)
        draw_wall(11*i,0);
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    for(int i = 0; i < map_width(); i++)
        draw_wall(11*i, (map_height()-1)*11);
    add_wall(0,              0,              VERTICAL,   map_height());
    for(int i = 0; i < map_height(); i++)
        draw_wall(0,11*i);
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    for(int i = 0; i < map_height(); i++)
        draw_wall(11*(map_width()-1),11*i);
    pc.printf("Walls done!\r\n");
    
    add_plant(10, 5);
    draw_plant(110, 55);
    //pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
   
    add_door(20,10);
    draw_door(220, 110);
    /*
    for(int k=0; k<5; k++){
        uLCD.filled_rectangle(11*(30+k-1),0,11*(31+k-1),110, WHITE);
        add_NPC2(30+k,10);
        draw_speaker(11*(30+k),110);
        }
    */
    add_NPC2(30,10);
    draw_speaker(330,110);   
    add_stone(18,30);
    draw_stone(18,30);
    add_big(10,6);
    draw_big(10,6);
    print_map();
}


void init_main_map2()
{
    uLCD.filled_rectangle(0,0,550,550, WHITE);
    Map* map = set_active_map(0);
    add_wall(0,              0,              HORIZONTAL, map_width());
    for(int i = 0; i < map_width(); i++)
        draw_wall(11*i,0);
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    for(int i = 0; i < map_width(); i++)
        draw_wall(11*i, (map_height())*11);
    add_wall(0,              0,              VERTICAL,   map_height());
    for(int i = 0; i < map_height(); i++)
        draw_wall(0,11*i);
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    for(int i = 0; i < map_height(); i++)
        draw_wall(11*(map_width()),11*i);
    pc.printf("Walls done!\r\n");
    
    add_attacker(8,10);
    draw_attacker(88,110);
    add_attacker(18,30);
    draw_attacker(198,330);
    add_plant(10, 5);
    draw_plant(110, 55);
    //pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
   
    add_door(20,10);
    draw_door(220,110);
    /*
    for(int k=0; k<5; k++){
        uLCD.filled_rectangle(11*(30+k-1),0,11*(30+k),110, WHITE);
        add_NPC1(30+k,10);
        draw_speaker(11*(30+k),110);
        }
        */
    add_NPC1(30,10);
    draw_speaker(330,110);   
    print_map();
}

    
    
    
void load(){
   // sd.read();
   FILE *fp = fopen("desktop/a.txt", "r");
   if ( fp != NULL ){
    char s[ 12 ];
    while ( fgets ( s, sizeof(s), fp ) != NULL ){
        fputs (s, fp );
        if (s[0]=='X'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.x=a;
        }
        if (s[0]=='Y'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.y=a;
        }
        if (s[0]=='x'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.px=a;
        }
        if (s[0]=='y'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.py=a;
        }
        if (s[0]=='H'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.health=a;
        }
        if (s[0]=='T'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.task=a;
        }
        if (s[0]=='O'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.Orange=a;
        }
        if (s[0]=='E'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.end=a;
        }
        if (s[0]=='K'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.has_key=a;
        }
        if (s[0]=='A'){
            s[0]='0';
            s[1]='0';
            int a=  atoi(s);
            Player.attack=a;
        }
        
        }
    }
    fclose(fp);

    }


    
    
void music() {
    FILE *wave_file;
    
    wave_file=fopen("/sd/background.wav","r");
    waver.play(wave_file);
    fclose(wave_file); 
  }

void check_status(){
    MapItem* Here = get_here(Player.x,Player.y);
    if(Here->type == orange){
        if (Player.health<=130){
                speech("Eat one orange", "Health add 20");
                Player.health+=20;
                deleteHere(Player.x,Player.y);
                }
            else
                speech("You now no need eat it","Have fun");
    }else if(Here->type == KEY){
        deleteHere(Player.x,Player.y);
        Player.has_key = 1;   
    }else if(Here->type == ATTACKER){
        speech("you are a mouse","fight with me");
    }
    }


int start(){
     uLCD.locate(16,16);
     uLCD.printf("Welcome the game, please push the bottom 1 continue" );
    int starting= !Button(read_inputs());
    return starting;
    }


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
 
 
 
int main()
{
    int starting= 1;
    // First things first: initialize hardware
    
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
   
    while( starting)
        starting=start();
     uLCD.cls();
   // music();
    maps_init();
    init_main_map1();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.health = 150;
    Player.Orange = 0;
    Player.has_key = 0;
    Player.attack=50;
    Player.end=2;
    Player.task=0;
    // Initial drawing
    draw_game(true);

    // Main game loop
    while(Player.end==2 && c==0 )
    {
        // Timer to measure game update speed
         // Actuall do the game update:
        // 1. Read inputs        
        // 2. Determine action (get_action)        
        // 3. Update game (update_game)
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        
        // 5. Frame delay
        Timer t; t.start();
        //draw_game(true);
        uLCD.locate(0,1);
        uLCD.printf("Status:");
        uLCD.locate(0,2);
        uLCD.printf("X:%d", Player.x);
        uLCD.locate(4,2);
        uLCD.printf("Y:%d", Player.y);
        uLCD.locate(0,3);
        uLCD.printf("Health:%d", Player.health);
        uLCD.locate(0,4);
        uLCD.printf("Orange:%d", Player.Orange);

        a=1;
        update_game(get_action(read_inputs()));
        draw_game(true);
        check_status();
        if(Player.health <=0)
        {
            Player.end=0;
        }
        t.stop();
        
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
    if (Player.end==1){
        maps_init();
        init_main_map2();
    
        // Initialize game state
        set_active_map(0);
        Player.x = Player.y = 5;
        Player.health = 150;
        Player.Orange = 0;
        Player.has_key = 0;
        Player.attack=50;
        Player.end=2;
        Player.task=0;
        // Initial drawing
        draw_game(true);
        while(Player.end==2){
        // Timer to measure game update speed
         // Actuall do the game update:
        // 1. Read inputs        
        // 2. Determine action (get_action)        
        // 3. Update game (update_game)
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        
        // 5. Frame delay
        Timer t; t.start();
        //draw_game(true);
        update_game(get_action(read_inputs()));
        draw_game(true);
        void check_status();
        
        uLCD.locate(0,1);
        uLCD.printf("Status:");
        uLCD.locate(8,1);
        uLCD.printf("X:%d", Player.x);
        uLCD.locate(4,1);
        uLCD.printf("Y:%d", Player.y);
        uLCD.locate(12,1);
        uLCD.printf("Health:%d", Player.health);
        uLCD.locate(16,1);
        uLCD.printf("Orange:%d", Player.Orange);
        a=2;
        if(Player.health <=0)
        {
            Player.end=0; 
        }
        t.stop();
        
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
    if (Player.end==1)
        draw_success();
    else if (Player.end==0)
        draw_fail(); 
    }
    else if (Player.end==0)
        draw_fail(); 
}
 
 

/*
int main()
{
    int starting= 1;
    // First things first: initialize hardware
    
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
   
    while( starting)
        starting=start();
    Thread t2(music);
    Thread t3(map1);
)
*/