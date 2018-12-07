#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
 
int get(int x){
    if (x/10==0)
        return x;
    else
        return x%10+31*(x/10);
}


static unsigned XY_KEY(int X, int Y) {
    int XY_KEY=get(X)+get(Y)*31*31;
    return XY_KEY;  // MAKE THE KEY 
}



/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    unsigned int n= key % 50;
    return n;
}

void maps_init()
{
     map.w=map_width();
     map.h=map_height();
     map.items= createHashTable(&map_hash, 50);
}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P','D','A','O'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return 50;
}

int map_height()
{
    return 50;
}

int map_area()
{
    return map_height()*map_width();
}

MapItem* get_north(int x, int y)
{
    MapItem* loc;
    int key=XY_KEY(x,y-1);
    loc=  (MapItem*) getItem(map.items, key);
    return loc;
}
 
MapItem* get_south(int x, int y)
{
     MapItem* loc;
     int key=XY_KEY(x,y+1);
     loc= (MapItem*)getItem(map.items, key);
     return loc;
}

MapItem* get_east(int x, int y)
{
     MapItem* loc;
     int key=XY_KEY(x+1,y);
     loc=(MapItem*)getItem(map.items, key);
     return loc;
}

MapItem* get_west(int x, int y)
{
     MapItem* loc;
     int key=XY_KEY(x-1,y);
     loc=(MapItem*)getItem(map.items, key);
     return loc;
}

MapItem* get_here(int x, int y)
{
    MapItem* loc;
    int key= XY_KEY(x,y);
    loc=(MapItem*)getItem(map.items, key);
    return loc;
}


void map_erase(int x, int y)
{
    uLCD.cls();
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = 0;
        w1->data = NULL;
        w1->attack=0;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = 0;
    w1->data = NULL;
     w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}



void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = 0;
    w1->data = 0;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_door_o(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = 1;
    w1->data = 0;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_attacker(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ATTACKER;
    w1->draw = draw_attacker;
    w1->walkable = 1;
    int life=100;
    w1->data = &life;
    w1->attack=100;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC1(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC1;
    w1->draw = draw_speaker;
    w1->walkable = 1;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC2;
    w1->draw = draw_speaker;
    w1->walkable = 1;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_orange(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = orange;
    w1->draw = draw_orange;
    w1->walkable = 1;
    int life=50;
    w1->data = &life;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_plant_dead(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANTD;
    w1->draw = draw_plant_dead;
    w1->walkable = 0;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = 1;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void deleteHere(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->draw = draw_nothing;
    w1->type = NULL;
    w1->walkable = 1;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free i
    }


void add_stone(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = stone;
    w1->draw = draw_stone;
    w1->walkable = 1;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_big(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BIG;
    w1->draw = draw_big;
    w1->walkable = 0;
    w1->data = NULL;
    w1->attack=0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}




