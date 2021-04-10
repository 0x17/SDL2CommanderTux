/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Actor.cpp
// For both player and enemies
// Based upon the CSprite class

#include "Actor.h"

//
// CActor
// Constructor
//
CActor::CActor( int s_x, int s_y, SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES], int fall_speed )
: CSprite( NULL )
{
    int i;
    
    // Init vars
    m_level = level;
    m_s_x = s_x;
    m_s_y = s_y;
    m_fall_speed = fall_speed;
    
    // Load all images
    for( i=0; i<NUM_ACTOR_IMAGES; i++ )
        m_images[i] = images[i];
    
    // Setup the rectangle
    m_rect.w = TILE_W;
    m_rect.h = TILE_H;
    
    SetPos( m_s_x, m_s_y );
    
    // Normally the actor stands
    SetImage( m_images[STANDING_RIGHT] );
    
    // Some var initializing    
    m_flip = true;
    m_fall.active = m_jump.active = false;
    m_dir = DIR_RIGHT;
    m_standing = false;
    m_reset_image = false;
    m_jump_increase = 0;
    
    m_dying = false;
}

//
// GetDying
//
bool CActor::GetDying()
{
    return m_dying;
}

//
// Die
//
void CActor::Die()
{
    if( !m_dying ) // Prevent from dying more than once
    {
        m_dying = true;
        
        // Set suitable dying image
        if( m_dir == DIR_RIGHT )        
            SetImage( m_images[DEAD_RIGHT] );
        else
            SetImage( m_images[DEAD_LEFT] );
    }
}

//
// Respawn
//
void CActor::Respawn()
{
    // Only respawn when dying
    if( m_dying )
    {
        SetImage(m_images[STANDING_RIGHT] );
        m_dying = false;
        // Get back to old start position
        m_rect.y = m_s_y;
        m_rect.x = m_s_x;
    }
}

//
// Walk
//
void CActor::Walk( Direction dir, int walk_speed )
{
    int i;
    
    // Dead actors can't walk
    if( !m_dying )
    {
        // We're not standing anymore
        m_standing = false;
    
        // Update image to current direction if neccessary
        if( !m_jump.active && !m_fall.active )
        {
            // Walk left
            if( dir == DIR_LEFT )
            {
                // Flip the image after a specific time
                if( g_framework->GetTicks() - m_last_flip > FLIP_TIME )
                {
                    // Flip from false to true
                    // Standing left image to walking left image
                    if( !m_flip )
                    {
                        SetImage( m_images[WALKING_LEFT] );
                        m_flip = true;
                        
                    }
                    // Flip from true to false
                    // Walking left image to standing left image
                    else
                    {
                        SetImage( m_images[STANDING_LEFT] );
                        m_flip = false;
                    }
                    
                    // Refresh the time
                    m_last_flip = g_framework->GetTicks();
                }
            }
            // Walk right
            else
            {
                // Flip the image after a specific time
                if( g_framework->GetTicks() - m_last_flip > FLIP_TIME )
                {
                    // Flip from false to true
                    // Standing right image to walking right image
                    if( !m_flip )
                    {
                        SetImage( m_images[WALKING_RIGHT] );
                        m_flip = true;
                        
                    }
                    // Flip from true to false
                    // Walking right image to standing right image
                    else
                    {
                        SetImage( m_images[STANDING_RIGHT] );
                        m_flip = false;
                    }
                    
                    // Refresh the time
                    m_last_flip = g_framework->GetTicks();
                }
            }
        
        }
        else
        {
            // Direction changed?
            if( m_dir != dir )
            {
                // Update jump image
                if( dir == DIR_LEFT )
                    SetImage( m_images[JUMPING_LEFT] );
                else
                    SetImage( m_images[JUMPING_RIGHT] );
            }
        }
    
        // Walk if possible
        for( i=0; i<walk_speed; i++ )
        {
            // Approximate
            if( dir == DIR_RIGHT )
            {
                if( WalkPossible( dir ) )
                    m_rect.x++;
                else
                    break;
            }
            else
            {
                if( WalkPossible( dir ) )
                    m_rect.x--;
                else
                    break;
            }
        }
        
        // Copy to member var
        m_dir = dir;
    
    } // !m_dying
}

//
// WalkPossible
// Check whether walking is possible
//
// At all 2 Tiles can be beside the player
// The player can walk when both of them are empty
//
bool CActor::WalkPossible( Direction dir )
{
    // Can the actor walk right?    
    if( dir == DIR_RIGHT )
    {
        if( m_level->content[m_rect.y/TILE_H]
                            [(m_rect.x+m_rect.w+1)/TILE_W]
            == TILE_EMPTY &&
            m_level->content[(m_rect.y+m_rect.h-1)/TILE_H]
                            [(m_rect.x+m_rect.w+1)/TILE_W]
            == TILE_EMPTY &&
            m_rect.x + m_rect.w < LVL_W_MAX * TILE_W )
            return true; // Walking right possible
    }
    else // Or left?
    {
        if( m_level->content[m_rect.y/TILE_H]
                            [(m_rect.x-1)/TILE_W]
            == TILE_EMPTY &&
            m_level->content[(m_rect.y+m_rect.h-1)/TILE_H]
                            [(m_rect.x-1)/TILE_W]
            == TILE_EMPTY &&
            m_rect.x > 0 )
            return true; // Walking left possible
    }
    
    // Walking not possible
    return false;
}

//
// Jump
//
void CActor::Jump()
{
    // Only jump when not falling or jumping yet
    if( !m_jump.active && !m_fall.active && !m_dying )
    {
        // Set all values for a new jump
        // Collisions stuff is all handled in the Update() func!
        // Since we are not in UT 2004 actors can't start jumping in the air...
        {
            m_jump.active = true;
            
            // Refresh time
            m_jump.s_time = g_framework->GetTicks();
            
            // Set correct image
            if( m_dir == DIR_RIGHT )
                SetImage( m_images[JUMPING_RIGHT] );
            else
                SetImage( m_images[JUMPING_LEFT] );
        }
    }
}

//
// StopJump
//
void CActor::StopJump( bool bump )
{
    // Can't stop when not actually jumping
    if( m_jump.active )
    {
        // Reset vars
        m_jump.active = false;
        m_reset_image = false;
        m_jump_increase = 0;
    }
}

//
// IncreaseJump
// So holding the jump key longer increases the jump
//
void CActor::IncreaseJump( bool ignore_max )
{
    // Only increase up to a specific value except ignore max is true
    if( m_jump.active )
    {
        if( m_jump_increase < JUMP_INCREASE_MAX ||
            ignore_max )
            m_jump_increase+=JUMP_INCREASE_POWER;
    }
}

//
// Update
//
void CActor::Update( int *scroll_x, int *scroll_y, bool enemy )
{
    unsigned int i;
    
    // Don't update dying actors in most cases
    if( !m_dying )
    {    
        // Jumping
        // As when walking you need to consider that at most two tiles
        // are near the actor
        if( m_jump.active )
        {
            // Not standing anymore
            m_standing = false;
                
            // Prevent from negative values and wrong numbers!
            if( ( (g_framework->GetTicks()-m_jump.s_time) / 20 ) < JUMP_POWER + m_jump_increase )
            {
                // Don't jump inside of the tiles above
                // Approximation
                for( i=0;
                    i<(JUMP_POWER+m_jump_increase)-((g_framework->GetTicks()-m_jump.s_time)/20);
                    i++ )
                {
                    // Jump possible
                    if( m_level->content[(m_rect.y-1)/TILE_H]
                                        [(m_rect.x+1)/TILE_W]
                        == TILE_EMPTY &&
                        m_level->content[(m_rect.y-1)/TILE_H]
                                        [(m_rect.x+m_rect.w-1)/TILE_W]
                        == TILE_EMPTY )
                    {
                        m_rect.y--;
                    }
                    // Bump against the roof
                    else
                    {
                        StopJump( true );                        
                        break;
                    }
                }            
            }
            // Jump power is exceeded, the end is reached
            else
                StopJump( false );
        }
        
        // Not jumping anymore... reset image!
        if( m_standing )
        {
            m_reset_image = true;
            // Once is enough!
            m_standing = false;
        }
        
        // Look whether the actor needs to fall
        if( !m_jump.active && !m_fall.active )
        {
            // Is the tile under us empty?
            if( m_level->content[(m_rect.y+m_rect.h+OVER)/TILE_H]
                                [(m_rect.x+1)/TILE_W]
                == TILE_EMPTY &&
                m_level->content[(m_rect.y+m_rect.h+OVER)/TILE_H]
                                [(m_rect.x+m_rect.w-1)/TILE_W]
                == TILE_EMPTY )
            {                
                m_fall.active = true;
                m_fall.s_time = g_framework->GetTicks();
                    
                // Set to the correct image
                if( m_dir == DIR_LEFT )
                    SetImage( m_images[JUMPING_LEFT] );
                else
                    SetImage( m_images[JUMPING_RIGHT] );
            }
        }
        
        // Reset image after falling or something
        if( m_reset_image )
        {
            // Only if not falling
            if( !m_fall.active )
            {
                // Make tux only look jumping when he really is!
                if( m_dir == DIR_LEFT )
                    SetImage( m_images[STANDING_LEFT] );
                else
                    SetImage( m_images[STANDING_RIGHT] );
            }
                
            m_reset_image = false;
        }
        
    } // !m_dying
    
    // Falling    
    if( m_fall.active && !m_jump.active )
    {
        // Enemies still fall when dead
        if( !m_dying || enemy )
        {
            // Approximation
            for( i=0;
                i<m_fall_speed+( (g_framework->GetTicks() - m_fall.s_time) / 100 );
                i++ )
            {
                // Is the tile under us empty?
                if( m_level->content[(m_rect.y+m_rect.h+OVER)/TILE_H]
                                    [(m_rect.x+1)/TILE_W]
                    == TILE_EMPTY &&
                    m_level->content[(m_rect.y+m_rect.h+OVER)/TILE_H]
                                        [(m_rect.x+m_rect.w-1)/TILE_W]
                    == TILE_EMPTY &&
                    m_rect.y + m_rect.h < LVL_H_MAX * TILE_H )
                {
                    m_rect.y++;                    
                }
                // Actor is falling out of the level
                else if( m_rect.y + m_rect.h >= LVL_H_MAX * TILE_H )
                {
                    // Dying...
                    Die();
                }
                // Actor bumps into the ground
                else
                {
                    // Stopping the fall
                    m_fall.active = false;
                    m_reset_image = true;
                    break;
                }
            } 
        }
    }
}
