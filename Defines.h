/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Defines.h
// Contains most #define stuff and all filenames

#ifndef DEFINES_H
#define DEFINES_H

#define VERSION     0.300000
#define VERSION_C   "3rd Alpha (0.3)"

#define DATA_PREFIX "data"

// Player images in the worldmap
#define NUM_W_P_IMAGES 8

#define NUM_LEVELS      4
#define NUM_TILES       24
#define NUM_ENEMIES     6
#define NUM_SONGS       2

// Disable stuff
//#define NO_SOUND

// Debugging macros
// use gcc -D ... to change these values without touching the sources
#define SHOW_DEBUG      1

#if SHOW_DEBUG == 1
#define DEBUG( msg ) printf( "CTux_Msg: " ); printf( msg ); printf( "\n" );
#define SHOW_VERSION() printf(  "CTux_Msg: " ); \
                       printf( "Version: %f", VERSION );\
                       printf( "\n" );
#else
    #define DEBUG( msg )
    #define SHOW_VERSION()
#endif

// Screen resolutions
#define SCR_W       640
#define SCR_H       480

#define EDIT_W      1024
#define EDIT_H      768

// Pseudo physics
#define PLAYER_FALL_SPEED     8

// Changing these values would prevent you from loading level files created by
// older versions of the game!!!
#define LVL_W_MAX   256 // array: 0-255
#define LVL_H_MAX   64 // array: 0-63

#define TILE_H      30
#define TILE_W      30

#define TILE_EMPTY  255

struct Position {
    int x, y;
    
    Position()
    { x=y=0; }
};

enum Direction {
    // Horizontal
    DIR_LEFT,
    DIR_RIGHT,
    
    // Vertical
    DIR_UP,
    DIR_DOWN
};

// Player

// This is interesting for cheaters...
#define NUM_LIVES           4

#define WALK_SPEED          7

#define JUMP_POWER          6
#define JUMP_INCREASE_MAX   15
#define JUMP_INCREASE_POWER 5

#define PLAYER_RECT_X   240
#define PLAYER_RECT_Y   80
#define PLAYER_RECT_W   160
#define PLAYER_RECT_H   160

#define SCROLL_SPEED    WALK_SPEED

#define OVER            0

#define FLIP_TIME       150 // msec...

// Gravity
struct Gravity_Event {
    Uint32 s_time;
    bool active;
    
    Gravity_Event() 
    {
        s_time = 0;
        active = false;
    }
};

#endif
