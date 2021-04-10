/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Actor.h

#ifndef ACTOR_H
#define ACTOR_H

#include "Sprite.h"
#include "SLevel.h"

// Anything concerning to the level should derive from this class
class CActor : public CSprite
{    
    public:
        CActor( int s_x, int s_y,
				SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES],
				int fall_speed );
        
        bool GetDying();
        virtual void Die();
        virtual void Respawn();
        
        virtual void Walk(  Direction dir=DIR_LEFT, int walk_speed = WALK_SPEED );
        bool WalkPossible( Direction dir );
        
        virtual void Jump();       
        virtual void StopJump( bool bump );
        void IncreaseJump( bool ignore_max = false );
        
        virtual void Update( int *scroll_x, int *scroll_y, bool enemy );
        
        bool GetFalling() { return m_fall.active; }
        void SetFalling( bool value ) { m_fall.active = value; }
        
        bool GetStanding() { return m_standing; }
        void SetStanding( bool value ) { m_standing = value; }
        
        void SetDirection( Direction dir ) { m_dir = dir; }
        Direction GetDirection() { return m_dir; }
        
        bool IsJumping() { return m_jump.active; }
        
    protected:
        int m_s_x, m_s_y;
        
        SDL_Texture *m_images[NUM_ACTOR_IMAGES];
        
        Direction m_dir;
        SLevel *m_level;
        
        bool m_flip;
        Uint32 m_last_flip;
        
        int m_fall_speed;
        Gravity_Event m_jump, m_fall;
        unsigned int m_jump_increase;
        bool m_reset_image;
        bool m_standing, m_dying;
};

#endif
