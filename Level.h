/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Level.h

#ifndef LEVEL_H
#define LEVEL_H

#include "Framework.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "SLevel.h"
#include "Text.h"
#include "Load.h"

#define MAX_ITEMS       100

/* because the WalkPossible() func of CPlayer
   normally forbids collision with non TILE_EMPTY */
#define END_OVERLAY     10


/*
    While the SLevel structure holds all the data saved into the level files
    This class delivers the functions needed to work with levels.
*/
class CLevel
{
    public:
        CLevel( const char *filename = NULL,
                bool editor = false,
                bool duel = false,
                const char *background_filename = background_filenames[BACKGROUND_1] );
        ~CLevel();
        
        void Load( const char *filename );
        void Save( const char *filename );
        
        void Update( int *scroll_x, int *scroll_y );
        void GetStartPos( int *x, int *y );
        void Draw( int *scroll_x, int *scroll_y, int w, int h, bool hide_players );
        
        SDL_Texture *GetTile( int tile_nr ) { return m_tiles[tile_nr]; }
        SDL_Texture *GetEnemySurf( int enemy_nr ) { return m_enemy_surfaces[enemy_nr][0]; }
        SDL_Texture *GetItemSurf( int item_nr ) { return m_item_surfaces[item_nr]; }
        
        CPlayer *GetPlayer( int p_num=0 ) { return m_players[p_num]; }
        
        SLevel *GetLevelStruct() { return m_level; };
        
        CEnemy *AddEnemy( int enemy_nr, int x, int y );
        CItem *AddItem( int item_nr, int x, int y );
        
        void PrintLevelContent();
        
        SDL_Rect *GetEndRect() { return &m_end_rect; }
        
        void GetBorderPoint( int *x, int *y );
    private:
        
        SDL_Texture *m_load_background, *m_load_circle;

        Position m_border;
        
        bool m_gameover;
        
        SDL_Rect m_end_rect;
        
        bool m_editor, m_duel;
        bool m_hide_players; // for editor mode
        
        SLevel *m_level;
        
        SDL_Texture *m_background;
        SDL_Texture *m_tiles[NUM_TILES];
        
        SDL_Texture *m_item_surfaces[NUM_ITEMS];
        
        SDL_Texture *m_enemy_surfaces[NUM_ENEMIES][NUM_ACTOR_IMAGES];
        SDL_Texture *m_player_surfaces[MAX_PLAYERS][NUM_ACTOR_IMAGES];
        SDL_Texture *m_bullet_surf, *m_gun_surf[2];

#ifndef NO_SOUND
        Mix_Chunk *m_player_snds[NUM_PLAYER_SOUNDS];
#endif
        CLinkedList< CEnemy > *m_enemies;
        CLinkedList< CItem > *m_items;
        
        CPlayer *m_players[MAX_PLAYERS];        
};

#endif
