/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Filenames.h

#ifndef FILENAMES_H
#define FILENAMES_H

#include "Defines.h"

// Load
static char *load_filenames[2] = {
    DATA_PREFIX "/images/load/background.bmp",
    DATA_PREFIX "/images/load/circle.bmp"
};

enum {
    LD_BACKGROUND,
    LD_CIRCLE
};

// Worldmap
static char *worldmap_filenames[1] = {
    DATA_PREFIX "/images/worldmap/world.jpg"
};
enum { WORLD };

static char *w_player_filenames[2][NUM_W_P_IMAGES] = {
    {
        DATA_PREFIX "/images/worldmap/player/standing/0.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/45.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/90.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/135.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/180.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/225.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/270.bmp",
        DATA_PREFIX "/images/worldmap/player/standing/315.bmp"
    },
    {
        DATA_PREFIX "/images/worldmap/player/walking/0.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/45.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/90.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/135.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/180.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/225.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/270.bmp",
        DATA_PREFIX "/images/worldmap/player/walking/315.bmp"
    }
};

enum {
    STANDING_0,
    STANDING_45,
    STANDING_90,
    STANDING_135,
    STANDING_180,
    STANDING_225,
    STANDING_270,
    STANDING_315,
    
    WALKING_0,
    WALKING_45,
    WALKING_90,
    WALKING_135,
    WALKING_180,
    WALKING_225,
    WALKING_270,
    WALKING_315,   
};

static char *worldmap_level_done_filename[] = {
    DATA_PREFIX "/images/worldmap/levels/done.bmp"
};

enum {
    LVL_DONE
};

static char *worldmap_level_filenames[NUM_LEVELS] = {
    DATA_PREFIX "/images/worldmap/levels/level1.bmp",
    DATA_PREFIX "/images/worldmap/levels/level2.bmp",
    DATA_PREFIX "/images/worldmap/levels/level3.bmp",
    DATA_PREFIX "/images/worldmap/levels/level4.bmp"
};

enum {
    W_LEVEL_1,
    W_LEVEL_2,
    W_LEVEL_3,
    W_LEVEL_4
};

#ifndef NO_SOUND
// Songs

static char *song_filenames[NUM_SONGS] = {
    DATA_PREFIX "/music/techi.ogg",
    DATA_PREFIX "/music/strange.ogg"
};

enum {
    SNG_TECHI,
    SNG_STRANGE
};

// Player sounds

#define NUM_PLAYER_SOUNDS           8

static char *player_sound_filenames[NUM_PLAYER_SOUNDS] = {
    DATA_PREFIX "/sounds/player_walking.wav",
    DATA_PREFIX "/sounds/player_jump.wav",
    DATA_PREFIX "/sounds/player_dying.wav",
    DATA_PREFIX "/sounds/player_killing.wav",
    DATA_PREFIX "/sounds/player_bump.wav",
    DATA_PREFIX "/sounds/player_shooting.wav",
    DATA_PREFIX "/sounds/player_item_collected.wav",
    DATA_PREFIX "/sounds/player_level_done.wav"
};

enum {
    PSND_WALKING,
    PSND_JUMP,
    PSND_DYING,
    PSND_KILLING,
    PSND_BUMP,
    PSND_SHOOTING,
    PSND_ITEM_COLLECTED,
    PSND_LEVEL_DONE
};

#endif // NO_SOUND

// Duel levels
static char *duel_lvl_filenames[] = {
    DATA_PREFIX "/levels/duel/level1.ctux"
};

enum {
    DUEL_LVL_1
};

// Fonts
static char *font_filenames[] = {
    DATA_PREFIX "/fonts/font.ttf"
};

enum {
    FNT_FONT
};

// Splash
static char *splash_filenames[] = {
    DATA_PREFIX "/images/splash.jpg",
    DATA_PREFIX "/sounds/splash_sound.wav"
};

enum {
    SPL_SPLASH,
    SPL_SOUND
};

// Menu
static char *menu_filenames[] = {
    DATA_PREFIX "/images/menu/menu.bmp",
    DATA_PREFIX "/images/menu/head.bmp",
    DATA_PREFIX "/images/menu/settings/menu.bmp",
    DATA_PREFIX "/sounds/change_selection.wav"
};

enum {
    MN_MENU,
    MN_HEAD,
    MN_SETTINGS_MENU,
    MN_SEL_SND
};

// Credits
static char *credits_filenames[] = {
    DATA_PREFIX "/images/credits/me.bmp",
    DATA_PREFIX "/images/credits/dogs.bmp"
};

enum {
    CREDITS_ME,
    CREDITS_DOGS
};

// Player
// w = tile_w, h = tile_h !!

#define MAX_PLAYERS     4
#define NUM_ACTOR_IMAGES    8

static char *player_filenames[MAX_PLAYERS][NUM_ACTOR_IMAGES] = {
    {
        DATA_PREFIX "/images/player1/standing_right.bmp",
        DATA_PREFIX "/images/player1/walking_right.bmp",
        DATA_PREFIX "/images/player1/jumping_right.bmp",
        DATA_PREFIX "/images/player1/dead_right.bmp",
        
        DATA_PREFIX "/images/player1/standing_left.bmp",
        DATA_PREFIX "/images/player1/walking_left.bmp",
        DATA_PREFIX "/images/player1/jumping_left.bmp",
        DATA_PREFIX "/images/player1/dead_left.bmp"
    },
    {
        DATA_PREFIX "/images/player2/standing_right.bmp",
        DATA_PREFIX "/images/player2/walking_right.bmp",
        DATA_PREFIX "/images/player2/jumping_right.bmp",
        DATA_PREFIX "/images/player2/dead_right.bmp",
        
        DATA_PREFIX "/images/player2/standing_left.bmp",
        DATA_PREFIX "/images/player2/walking_left.bmp",
        DATA_PREFIX "/images/player2/jumping_left.bmp",
        DATA_PREFIX "/images/player2/dead_left.bmp"
    },
    {
        DATA_PREFIX "/images/player3/standing_right.bmp",
        DATA_PREFIX "/images/player3/walking_right.bmp",
        DATA_PREFIX "/images/player3/jumping_right.bmp",
        DATA_PREFIX "/images/player3/dead_right.bmp",
        
        DATA_PREFIX "/images/player3/standing_left.bmp",
        DATA_PREFIX "/images/player3/walking_left.bmp",
        DATA_PREFIX "/images/player3/jumping_left.bmp",
        DATA_PREFIX "/images/player3/dead_left.bmp"
    },
    {
        DATA_PREFIX "/images/player4/standing_right.bmp",
        DATA_PREFIX "/images/player4/walking_right.bmp",
        DATA_PREFIX "/images/player4/jumping_right.bmp",
        DATA_PREFIX "/images/player4/dead_right.bmp",
        
        DATA_PREFIX "/images/player4/standing_left.bmp",
        DATA_PREFIX "/images/player4/walking_left.bmp",
        DATA_PREFIX "/images/player4/jumping_left.bmp",
        DATA_PREFIX "/images/player4/dead_left.bmp"
    }
};

// Bullet
static char *bullet_filenames[] = {
    DATA_PREFIX "/images/bullet.bmp"
};

enum { BULLET };

// Gun
static char *gun_filenames[] = {
    DATA_PREFIX "/images/gun_left.bmp",
    DATA_PREFIX "/images/gun_right.bmp"
};

enum { GUN_LEFT, GUN_RIGHT };

// Tiles

// New tiles have to be added at the end, so old levels still work!!!
static char *tile_filenames[NUM_TILES] = {
    DATA_PREFIX "/images/tiles/box_blue.bmp",
    DATA_PREFIX "/images/tiles/box_red.bmp",
    DATA_PREFIX "/images/tiles/box_green.bmp",
    DATA_PREFIX "/images/tiles/box_orange.bmp",
    DATA_PREFIX "/images/tiles/gradient_blue.bmp",
    DATA_PREFIX "/images/tiles/gradient_red.bmp",
    DATA_PREFIX "/images/tiles/gradient_green.bmp",
    DATA_PREFIX "/images/tiles/gray.bmp",
    DATA_PREFIX "/images/tiles/nin.bmp",
    DATA_PREFIX "/images/tiles/snow1.bmp",
    DATA_PREFIX "/images/tiles/snow2.bmp",
    DATA_PREFIX "/images/tiles/snow3.bmp",
    DATA_PREFIX "/images/tiles/brown_stone.bmp",
    DATA_PREFIX "/images/tiles/ice.bmp",
    DATA_PREFIX "/images/tiles/platform1.bmp",
    DATA_PREFIX "/images/tiles/platform2.bmp",
    DATA_PREFIX "/images/tiles/platform3.bmp",
    DATA_PREFIX "/images/tiles/start.bmp",
    DATA_PREFIX "/images/tiles/end.bmp",
    DATA_PREFIX "/images/tiles/end_visible.bmp",
    DATA_PREFIX "/images/tiles/grass1.bmp",
    DATA_PREFIX "/images/tiles/grass2.bmp",
    DATA_PREFIX "/images/tiles/grass3.bmp",
    DATA_PREFIX "/images/tiles/ground.bmp"
};

enum {
    TILE_BOX_BLUE,
    TILE_BOX_RED,
    TILE_BOX_GREEN,
    TILE_BOX_ORANGE,
    TILE_GRADIENT_BLUE,
    TILE_GRADIENT_RED,
    TILE_GRADIENT_GREEN,
    TILE_GRAY,
    TILE_NIN,
    TILE_SNOW1,
    TILE_SNOW2,
    TILE_SNOW3,
    TILE_BROWN_STONE,
    TILE_ICE,
    TILE_PLATFORM1,
    TILE_PLATFORM2,
    TILE_PLATFORM3,
    TILE_START,
    TILE_END,
    TILE_END_VISIBLE,
    TILE_GRASS1,
    TILE_GRASS2,
    TILE_GRASS3,
    TILE_GROUND
};

// Actor
enum {
    STANDING_RIGHT,
    WALKING_RIGHT,
    JUMPING_RIGHT,
    DEAD_RIGHT,
    
    STANDING_LEFT,
    WALKING_LEFT,
    JUMPING_LEFT,
    DEAD_LEFT
};

// Enemies
static char *enemy_filenames[NUM_ENEMIES][NUM_ACTOR_IMAGES] = {
    {   
        // Blob
        DATA_PREFIX "/images/enemies/blob/standing_right.bmp",
        DATA_PREFIX "/images/enemies/blob/walking_right.bmp",
        DATA_PREFIX "/images/enemies/blob/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/blob/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/blob/standing_left.bmp",
        DATA_PREFIX "/images/enemies/blob/walking_left.bmp",
        DATA_PREFIX "/images/enemies/blob/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/blob/dead_left.bmp"
    },
    {
        // Robot
        DATA_PREFIX "/images/enemies/robot/standing_right.bmp",
        DATA_PREFIX "/images/enemies/robot/walking_right.bmp",
        DATA_PREFIX "/images/enemies/robot/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/robot/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/robot/standing_left.bmp",
        DATA_PREFIX "/images/enemies/robot/walking_left.bmp",
        DATA_PREFIX "/images/enemies/robot/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/robot/dead_left.bmp"        
    },
    {
        // Stoney
        DATA_PREFIX "/images/enemies/stoney/standing_right.bmp",
        DATA_PREFIX "/images/enemies/stoney/walking_right.bmp",
        DATA_PREFIX "/images/enemies/stoney/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/stoney/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/stoney/standing_left.bmp",
        DATA_PREFIX "/images/enemies/stoney/walking_left.bmp",
        DATA_PREFIX "/images/enemies/stoney/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/stoney/dead_left.bmp"
    },
    {
        // TODO: mmmh...
        // Plant
        DATA_PREFIX "/images/enemies/plant/standing_right.bmp",
        DATA_PREFIX "/images/enemies/plant/walking_right.bmp",
        DATA_PREFIX "/images/enemies/plant/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/plant/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/plant/standing_left.bmp",
        DATA_PREFIX "/images/enemies/plant/walking_left.bmp",
        DATA_PREFIX "/images/enemies/plant/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/plant/dead_left.bmp"
    },
	{
		// Snail
		DATA_PREFIX "/images/enemies/snail/standing_right.bmp",
        DATA_PREFIX "/images/enemies/snail/walking_right.bmp",
        DATA_PREFIX "/images/enemies/snail/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/snail/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/snail/standing_left.bmp",
        DATA_PREFIX "/images/enemies/snail/walking_left.bmp",
        DATA_PREFIX "/images/enemies/snail/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/snail/dead_left.bmp"
	},
	{
		// RocketBall
		DATA_PREFIX "/images/enemies/rocketball/standing_right.bmp",
        DATA_PREFIX "/images/enemies/rocketball/walking_right.bmp",
        DATA_PREFIX "/images/enemies/rocketball/jumping_right.bmp",
        DATA_PREFIX "/images/enemies/rocketball/dead_right.bmp",
        
        DATA_PREFIX "/images/enemies/rocketball/standing_left.bmp",
        DATA_PREFIX "/images/enemies/rocketball/walking_left.bmp",
        DATA_PREFIX "/images/enemies/rocketball/jumping_left.bmp",
        DATA_PREFIX "/images/enemies/rocketball/dead_left.bmp"
	}
};

// Items
#define NUM_ITEMS 10
#define ITEM_SHIFT 600

static char *item_filenames[NUM_ITEMS] = {
    DATA_PREFIX "/images/items/fish.bmp",
    DATA_PREFIX "/images/items/burger.bmp",        
    DATA_PREFIX "/images/items/hotdog.bmp",
    DATA_PREFIX "/images/items/c_book.bmp",
    DATA_PREFIX "/images/items/cpp_book.bmp",
    DATA_PREFIX "/images/items/sdl_book.bmp",
    DATA_PREFIX "/images/items/drink.bmp",
    DATA_PREFIX "/images/items/nin_cd.bmp",
    DATA_PREFIX "/images/items/bullets.bmp",
    DATA_PREFIX "/images/items/live.bmp"
};

enum {
    ITEM_FISH,
    ITEM_BURGER,
    ITEM_HOTDOG,
    ITEM_C_BOOK,
    ITEM_CPP_BOOK,
    ITEM_SDL_BOOK,
    ITEM_DRINK,
    ITEM_NIN_CD,
    ITEM_BULLETS,
    ITEM_LIVE
};


// Backgrounds
static char *background_filenames[] = {
    DATA_PREFIX "/images/backgrounds/background1.jpg",
    DATA_PREFIX "/images/backgrounds/background2.jpg",
    DATA_PREFIX "/images/backgrounds/background3.jpg",
    DATA_PREFIX "/images/backgrounds/background4.jpg"
};

enum{
    BACKGROUND_1,
    BACKGROUND_2,
    BACKGROUND_3,
    BACKGROUND_4
};

// Editor
static char *editor_filenames[] = {
    DATA_PREFIX "/images/editor/selected_tile.bmp"
};

enum {
    ED_TILE_SELECTED
};

// Levels

static char *level_filenames[NUM_LEVELS] = {
    DATA_PREFIX "/levels/level1.ctux",
    DATA_PREFIX "/levels/level2.ctux",
    DATA_PREFIX "/levels/level3.ctux",
    DATA_PREFIX "/levels/level4.ctux"
};

enum {
    LVL_1,
    LVL_2,
    LVL_3,
    LVL_4
};

#endif
