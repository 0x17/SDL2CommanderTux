/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Worldmap.h

#include "Framework.h"
#include "State.h"

#define WORLD_SCROLL_SPEED     5

#define WORLD_PLAYER_RECT_X    100
#define WORLD_PLAYER_RECT_Y    100
#define WORLD_PLAYER_RECT_W    440
#define WORLD_PLAYER_RECT_H    280

#define WORLD_WALK_SPEED       5
#define WORLD_P_IMG_FLIP_TIME  500

#define STANDING    0
#define WALKING     1

#define NUM_WALKABLE_AREAS      3

class CWorldmap
{
    public:
        CWorldmap();      
        ~CWorldmap();
        
		void SetImages( SDL_Texture *world,
						SDL_Texture *player[2][NUM_W_P_IMAGES] );

        void Input(const Uint8 *keys );
        void Update();
        void Draw();        
        
        void SetActive( bool value ) { m_active = value; }
        bool GetActive() { return m_active; }
        
        int GetActLevel() { return m_act_level; }
        void SetActLevel( int level_nr ) { m_act_level = level_nr; }
        
        bool GetGameRunning() { return m_game_running; }
        void SetGameRunning( bool value ) { m_game_running = value; }
        
        void MovePlayer( int x, int y )
        {
            m_player_rect.x+=x;
            m_player_rect.y+=y;
        }
        
        void SetLevelDone( int level_nr ) { m_level_done[level_nr] = true; }
        
        int GetLives() { return m_player_lives; }
        void SetLives( int lives ) { m_player_lives = lives; }
        
        int GetScore() { return m_player_score; }
        void SetScore( int score ) { m_player_score = score; }
        
        int GetBullets() { return m_player_bullets; }
        void SetBullets( int bullets ) { m_player_bullets = bullets; }
        
    private:
        
        bool WalkPossible( int x, int y );
        
        SReleased m_released;
        
        SDL_Texture *m_world;
        SDL_Rect m_walkable_areas[NUM_WALKABLE_AREAS];
        
        SDL_Texture *m_player[2][NUM_W_P_IMAGES];
        int m_player_state;
        SDL_Rect m_player_rect, m_world_src_rect;
        int m_player_angle;
        bool m_flip;
        Uint32 m_last_flip;
        bool m_walking;
        
        SDL_Texture *m_level_surfaces[NUM_LEVELS], *m_level_done_surface;
        SDL_Rect m_level_rectangles[NUM_LEVELS];
        bool m_level_done[NUM_LEVELS];
        
        bool m_game_running;
        int m_scroll_x, m_scroll_y;
        int m_act_level;
        bool m_active;
        
        int m_player_score, m_player_lives, m_player_bullets;
};
