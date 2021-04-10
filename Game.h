/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Game.h

#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "Worldmap.h"
#include "Level.h"
#include "Sprite.h"
#include "Load.h"

class CMenu;

struct STextMSG
{
    bool visible;
    int remaining;
};

class CGame : public CState
{
    public:
        CGame( SFramework_Setup *setup );
        virtual ~CGame();
        
        virtual void Input();
        virtual void Update();
        virtual void Draw();
        
        //void NextLevel();
        void ChangeLevel( const char *filename );        
        void SetCurLevel( int value ) { m_cur_level = value; }
        
        bool GetAllDone() { return m_all_done; }
        void SetAllDone( bool value ) { m_all_done = value; }

		void LoadWorldmapSurfaces( bool first_time=false );
		void UnloadWorldmapSurfaces();
    private:
        SFramework_Setup *m_setup; // because we need to edit the original
                                   // and not the copy from the framework
        bool m_all_done;

        SReleased m_released;
        
        SDL_Texture *m_load_background, *m_load_circle;
        
        CMenu *m_menu, *m_settings_menu;
        
        STextMSG m_credits;
        SDL_Texture *m_me, *m_dogs;
        
		bool m_worldmap_surfaces_loaded;
        SDL_Texture *m_world_surface, *m_world_player_surfaces[2][NUM_W_P_IMAGES];
        
        CWorldmap *m_worldmap;
        CLevel *m_level;
        int m_cur_level;
        
        int m_scroll_x, m_scroll_y;
        
#ifndef NO_SOUND
        Mix_Music *m_songs[NUM_SONGS];
#endif
};

// Changes here should also be made in the
// menu image file. Look at: Defines.h
#define    SEL_NEW_GAME     0
#define    SEL_DUEL         1
#define    SEL_SETTINGS     2
#define    SEL_EDITOR       3
#define    SEL_CREDITS      4
#define    SEL_EXIT         5

// Changes here are interesting for the settings menu image file!
#define    SEL_FULLSCREEN   0
#define    SEL_WINDOWED     1

#define CREDITS_TIME    100

class CMenu
{
    public:
        CMenu( const char *img_name,
			   const char *head_name,
			   bool settings_menu );
        ~CMenu();
        
        void Draw();
        void ChangeSelection( int selection );
        int GetSelection();
        
        void SetVisible( bool value ) { m_visible = value; }
        bool GetVisible() { return m_visible; }
    private:
        bool m_settings_menu;
        SDL_Texture *m_image, *m_head_surf;
        CAnim_Sprite *m_head_spr;
        bool m_head_move_eye;
        int m_selection;
        bool m_visible;
#ifndef NO_SOUND
        Mix_Chunk *m_sel_sound;
#endif
};

#endif
