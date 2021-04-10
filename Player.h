/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include "Text.h"
#include "Bullet.h"

#define SCORE_MSG_DURATION      1000
#define FRAG_MSG_DURATION       1000

#define RELOAD_TIME             500
#define PLAYER_BULLET_SPEED     10

enum ScoreReasons
{
    ENEMY_KILLED,
    ITEM_COLLECTED
};

class CPlayer : public CActor
{
    public:
        
#ifndef NO_SOUND
        CPlayer( int s_x, int s_y, SLevel *level,
                 SDL_Texture *images[NUM_ACTOR_IMAGES],
                 Mix_Chunk *player_snds[NUM_PLAYER_SOUNDS],
                 int p_num, bool duel,
                 Position border,
                 CLinkedList< CEnemy > *enemies=NULL,
                 SDL_Texture *bullet_surf=NULL,
                 SDL_Texture *gun_surf[2]=NULL );
#else
        CPlayer( int s_x, int s_y, SLevel *level,
                 SDL_Texture *images[NUM_ACTOR_IMAGES],
                 int p_num, bool duel,
                 Position border,
                 CLinkedList< CEnemy > *enemies=NULL,
                 SDL_Texture *bullet_surf=NULL,
                 SDL_Texture *gun_surf[2]=NULL );
#endif

        virtual void Update( int *scroll_x, int *scroll_y );
        virtual void Draw( int *scroll_x, int *scroll_y, int text_y );
        
        virtual void Jump();
        virtual void StopJump( bool bump );
        virtual void Walk( Direction dir=DIR_LEFT, int walk_speed=WALK_SPEED );
        
        virtual void Die();
        virtual void Respawn( int *scroll_x );
        
        bool GetRespawn() { return m_respawned; }
        void SetRespawn( bool respawned ) { m_respawned = respawned; }
        
        void Frag( CPlayer *victim );
        
        bool GetActive() { return m_active; }
        void SetActive( bool value ) { m_active = value; }
        
        void SetScore( int score ) { m_score = score; }
        int GetScore() { return m_score; }
        void IncrementScore( int value, int x, int y, int reason );
                
        void SetLives( int lives ) { m_lives = lives; }
        int GetLives() { return m_lives; }
        
        bool GetDead() { return m_dead; }
        void SetDead( bool value ) { m_dead = value; }
        
        void Shoot();
        
        void AddBullets( int bullets ) { m_num_bullets+=bullets; }
        void AddLive() { m_lives++; }
        
        int GetNumBullets() { return m_num_bullets; }
        void SetNumBullets( int bullets ) { m_num_bullets = bullets; }

        void PlaySound( int sound_nr )
        {
#ifndef NO_SOUND
			Mix_PlayChannel( -1, m_player_snds[sound_nr], 0 );
#endif
		}        
    private:
#ifndef NO_SOUND
        Mix_Chunk *m_player_snds[NUM_PLAYER_SOUNDS];
        Uint32 m_last_time_walking_sound;
#endif
        bool m_respawned;
        Position m_border;
        bool m_duel;
        int m_p_num;
        Color m_text_color;
        int m_frags, m_score, m_lives;
        bool m_dead;
        bool m_lives_decremented;
        bool m_active;

        CLinkedList< CFlyingText > m_score_msgs, m_frag_msgs;
        CLinkedList< CEnemy > *m_enemies;
        CLinkedList< CBullet > m_bullets;

        Uint32 m_last_shot;
        SDL_Texture *m_bullet_surf;
        SDL_Texture *m_gun_surf[2];
        int m_num_bullets;
        bool m_shooting;
};

#endif
