/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Player.cpp

#include "Player.h"

//
// CPlayer
// Constructor
//
#ifndef NO_SOUND
CPlayer::CPlayer( int s_x, int s_y, SLevel *level,
                  SDL_Texture *images[NUM_ACTOR_IMAGES],
                  Mix_Chunk *player_snds[NUM_PLAYER_SOUNDS], int p_num, bool duel,
                  Position border, CLinkedList< CEnemy > *enemies,
                  SDL_Texture *bullet_surf, SDL_Texture *gun_surf[2] )
#else
CPlayer::CPlayer( int s_x, int s_y, SLevel *level,
                  SDL_Texture *images[NUM_ACTOR_IMAGES], int p_num, bool duel,
                  Position border, CLinkedList< CEnemy > *enemies,
                  SDL_Texture *bullet_surf, SDL_Texture *gun_surf[2] )
#endif
: CActor( s_x, s_y, level, images, PLAYER_FALL_SPEED )
{    
#ifndef NO_SOUND
	int i;

    for( i=0; i<NUM_PLAYER_SOUNDS; i++ )
        m_player_snds[i] = player_snds[i];
    m_last_time_walking_sound = g_framework->GetTicks();
#endif
    
    // Some var initializing
    m_p_num = p_num;
    m_duel = duel;
    m_border = border;
    m_respawned = false;
    m_enemies = enemies;
    m_last_shot = g_framework->GetTicks();
    m_bullet_surf = bullet_surf;
    m_num_bullets = 0;
    m_shooting = false;
    
    // Left
    m_gun_surf[0] = gun_surf[0];
    // Right
    m_gun_surf[1] = gun_surf[1];
    
    m_frags = m_score = 0;
    m_lives = NUM_LIVES;
    m_dead = false;
    m_lives_decremented = false;
    
    SetActive( true );    
  
    // Every player has his own color
    switch( m_p_num )
    {
        case 0:
            m_text_color = COLOR_YELLOW;
            break;
        case 1:
            m_text_color = COLOR_RED;
            break;
        case 2:
            m_text_color = COLOR_BLUE;
            break;
        case 3:
            m_text_color = COLOR_GREEN;
            break;
    }
}

//
// Update
//
void CPlayer::Update( int *scroll_x, int *scroll_y )
{
    int i;
    
    // Don't update active players!!!
    if( m_active )
    {
        // Standard actor update stuff    
        CActor::Update( scroll_x, scroll_y, false );
        
        // Players go to heaven, when they die
        if( m_dying )
        {
            // Fly up
            if( m_rect.y-*scroll_y > 0 )
                m_rect.y-=10;
            else // And then respawn
                Respawn( scroll_x );
        }
    
        if( !m_duel ) // no scrolling and score msgs in duel mode
        {    
            // Scrolling    
            if( !m_dying )
            {        
                // Right
                // Player is right beside the specified rectangle?
                if( m_rect.x-*scroll_x + m_rect.w > PLAYER_RECT_X + PLAYER_RECT_W )
                {
                    // Approximate to scroll speed
                    for( i=0; i<SCROLL_SPEED; i++ )
                    {           
                        // Only scroll when possible
                        if( *scroll_x+SCR_W+1 < m_border.x )
                            (*scroll_x)++;
                        else
                            break;
                    }
                }     
                // Left
                // Player is left beside the specified rectangle?
                if( m_rect.x-*scroll_x < PLAYER_RECT_X )
                {
                    // Approximate to scroll speed
                    for( i=0; i<SCROLL_SPEED; i++ )
                    {
                        // Only scroll when possible
                        if( *scroll_x-1 > 0 )
                            (*scroll_x)--;
                        else
                            break;
                    }
                }    
                // Down      
                // Player is under the specified rectangle?
                if( m_rect.y-*scroll_y + m_rect.h > PLAYER_RECT_Y + PLAYER_RECT_H )
                {
                    // Approximate to scroll speed
                    for( i=0; i<(m_rect.y-*scroll_y+m_rect.h)-(PLAYER_RECT_Y+PLAYER_RECT_H); i++ )        
                    {
                        // Only scroll when possible
                        if( *scroll_y + SCR_H + 1 < LVL_H_MAX * TILE_H )
                            (*scroll_y)++;
                        else
                            break;
                    }
                }
                // Up
                // Player is above the specified rectangle?
                if( m_rect.y-*scroll_y < PLAYER_RECT_Y )
                {
                    // Approximate to scroll speed
                    for( i=0; i<SCROLL_SPEED; i++ )
                    {
                        // Only scroll when possible
                        if( *scroll_y-1 > m_border.y )
                            (*scroll_y)--;
                        else
                            break;
                    }
                }
                
            } // !m_dieing
                
            
            // Score messages
            for( i=0; i<m_score_msgs.GetNumberOfElements(); i++ )
            {
                // Delete if time exceeded or something else
                if( !m_score_msgs.GetData( i )->Update( scroll_x, scroll_y ) )
                    m_score_msgs.Delete( i );
            }
                
        } // !mduel
        else
        {
            // Frag messages
            for( i=0; i<m_frag_msgs.GetNumberOfElements(); i++ )
            {
                // Delete if time exceeded or something else
                if( !m_frag_msgs.GetData( i )->Update( scroll_x, scroll_y ) )
                    m_frag_msgs.Delete( i );
            }
        }
        
        
        // Bullets
        for( i=0; i<m_bullets.GetNumberOfElements(); i++ )
        {            
            if( !m_bullets.GetData( i )->Update( scroll_x, scroll_y ) ) 
            {
                if( m_bullets.GetData( i )->EnemyKilled() )
                {
                    CEnemy *victim = m_bullets.GetData( i )->GetVictim();
                    IncrementScore( victim->GetScoreValue(),
                                    victim->GetRect()->x+victim->GetRect()->w/2-5,
                                    victim->GetRect()->y, ENEMY_KILLED );
                }
                
                m_bullets.Delete( i );
            }
        }
        
        // Don't show gun after a specific time
        if( g_framework->GetTicks() - m_last_shot > RELOAD_TIME )
            m_shooting = false;
    
    } // m_active
}

//
// Draw
//
void CPlayer::Draw( int *scroll_x, int *scroll_y, int text_y )
{
    int i;

	// Only active players, this is important for duel mode
    if( m_active )
    {
        // In duel mode it looks better with small font, since there are more
        // informations to show via text    
        static Font_Size size;
        if( !m_duel )
            size = F_SIZE_BIG;
        else
            size = F_SIZE_SMALL;
        
        // Draw the image at the right position
        CActor::Draw( scroll_x, scroll_y );
        
        // Draw bullets
        for( i=0; i<m_bullets.GetNumberOfElements(); i++ )
        {
            m_bullets.GetData( i )->Draw( scroll_x, scroll_y );
        }
        
        // Draw gun
        if( m_shooting )
        {
            if( m_dir == DIR_LEFT )
                g_framework->Draw( m_gun_surf[0], m_rect.x-*scroll_x, m_rect.y-*scroll_y );
            else
                g_framework->Draw( m_gun_surf[1], m_rect.x-*scroll_x, m_rect.y-*scroll_y );
        }
        
        // Show lives and score and bullets
        if( !m_duel )
        {
            // Lives
            g_framework->DrawShadowText( "Lives:", SCR_W-100, text_y,
                                        COLOR_BLACK, m_text_color, size );
            g_framework->DrawShadowNumber( m_lives, SCR_W-30, text_y,
                                        COLOR_BLACK, m_text_color, size );
                                        
            // Score msgs
            for( i=0; i<m_score_msgs.GetNumberOfElements(); i++ )
                m_score_msgs.GetData( i )->Draw( scroll_x, scroll_y );
            
            // Score
            g_framework->DrawShadowText( "Score:", SCR_W-100, text_y+30,
                                        COLOR_BLACK, m_text_color, F_SIZE_MEDIUM );
            g_framework->DrawShadowNumber( m_score, SCR_W-30, text_y+30,
                                        COLOR_BLACK, m_text_color, F_SIZE_MEDIUM );
            
            // Bullets
            g_framework->DrawShadowText( "Bullets:", SCR_W-100, text_y+45,
                                        COLOR_BLACK, m_text_color, F_SIZE_MEDIUM );
            g_framework->DrawShadowNumber( m_num_bullets, SCR_W-30, text_y+45,
                                        COLOR_BLACK, m_text_color, F_SIZE_MEDIUM );
        }
        else // Show frags
        {
            g_framework->DrawShadowText( "Frags:", SCR_W-100, text_y,
                                        COLOR_BLACK, m_text_color, size );
            g_framework->DrawShadowNumber( m_frags, SCR_W-30, text_y,
                                        COLOR_BLACK, m_text_color, size );
            
            // Frag msgs
            for( i=0; i<m_frag_msgs.GetNumberOfElements(); i++ )
                m_frag_msgs.GetData( i )->Draw( scroll_x, scroll_y );
        }      
    } // m_active
}

//
// Jump
//
void CPlayer::Jump()
{
    static bool temp;
    
    temp = m_jump.active;
    
    // First do normal jump routine
    CActor::Jump();
    
#ifndef NO_SOUND
    // Then play player's jump sound if jump started
    if( m_jump.active && !temp )   
        Mix_PlayChannel( -1, m_player_snds[PSND_JUMP], 0 );
#endif
}

//
// StopJump
//
void CPlayer::StopJump( bool bump )
{
    // Standard StopJump func
    CActor::StopJump( bump );
    
#ifndef NO_SOUND
    // Play sound
    if( bump )
        Mix_PlayChannel( -1, m_player_snds[PSND_BUMP], 0 );
#endif
}

//
// Walk
//
void CPlayer::Walk( Direction dir, int walk_speed )
{
    // Normal walking func
    CActor::Walk( dir, walk_speed );
    
#ifndef NO_SOUND
    // Play walking sound every 150 miliseconds if not falling
    if( g_framework->GetTicks() - m_last_time_walking_sound > 150 )
    {
        if( !GetFalling() )
        {
            Mix_PlayChannel( -1, m_player_snds[PSND_WALKING], 0 );
            m_last_time_walking_sound = g_framework->GetTicks();
        }
    }
#endif
}

//
// Frag
// Only for duel mode
//
void CPlayer::Frag( CPlayer *victim)
{
    // You cannot frag someone dying or inactive
    if( !victim->GetDying() && victim->GetActive() )
    {
        // Only frag when hitting the head
        static SDL_Rect temp_rect;
        
        // Head rect ( actually the hat )
        temp_rect.w = 17; temp_rect.h = 10;
        temp_rect.x = victim->GetRect()->x + 6;
        temp_rect.y = victim->GetRect()->y;
        
        // Collision with hat
        if( g_framework->Coll( &m_rect, &temp_rect ) )
        {
            CFlyingText *frag_msg;
            
            // Init the frag message
            frag_msg = new CFlyingText( "+1", victim->GetRect()->x + victim->GetRect()->w/2 - 5,
                                        victim->GetRect()->y, DIR_UP, FRAG_MSG_DURATION,
                                        COLOR_YELLOW );
            // Increment frags
            m_frags++;
            
            // Kill the victim
            victim->Die();
            
            // Add the frag message to the list
            m_frag_msgs.Add( frag_msg );
        }
    }
}

//
// Die
// Big difference to base function: A player has lives
//
void CPlayer::Die()
{
    // Standard die func
    CActor::Die();
    
    if( !m_duel )
    {
        // Prevent from decrementing more than once
        if( !m_lives_decremented )
        {
#ifndef NO_SOUND
            Mix_PlayChannel( -1, m_player_snds[PSND_DYING], 0 );
#endif
            
            if( m_lives > 0 )
                m_lives--;
            else
                m_dead = true;
            m_lives_decremented = true;
        }
    }
}

//
// Respawn
// Necessary to support CPlayer specific die function
//
void CPlayer::Respawn( int *scroll_x )
{
    // Standard respawn func
    CActor::Respawn();
    
    // Scroll to the right pos
    if( !m_duel )
        *scroll_x = m_s_x;
    
    // We can decrement the lives again
    m_lives_decremented = false;    
    
    m_respawned = true;
}

//
// IncrementScore
// x and y are for the score message
//
void CPlayer::IncrementScore( int value, int x, int y, int reason )
{
    CFlyingText *score_msg;
    
    // Copy function parameters to member vars
    m_score+=value;
    
    // Init the new score message
    score_msg = new CFlyingText( g_framework->NumToText( value ), x, y,
                                 DIR_UP, SCORE_MSG_DURATION, COLOR_YELLOW );
    
    // Add it to the linked list of messages
    m_score_msgs.Add( score_msg );

#ifndef NO_SOUND    
    if( reason == ENEMY_KILLED )
        Mix_PlayChannel( -1, m_player_snds[PSND_KILLING], 0 );
    else
        Mix_PlayChannel( -1, m_player_snds[PSND_ITEM_COLLECTED], 0 );
#endif
}

//
// Shoot
//
void CPlayer::Shoot()
{
    // Only if enough time passed till the last shot
    if( g_framework->GetTicks() - m_last_shot > RELOAD_TIME && m_num_bullets > 0 )
    {
        CBullet *new_bullet;
        
        // Allocate new bullet
        new_bullet =
            new CBullet( m_bullet_surf,
                         m_rect.x + m_rect.w/2, m_rect.y + m_rect.h/2, PLAYER_BULLET_SPEED,
                         m_dir, m_level, m_enemies );
            
        // Add it to the linked list of bullets
        m_bullets.Add( new_bullet );
        
        m_num_bullets--;
        m_last_shot = g_framework->GetTicks();
        
        m_shooting = true;

#ifndef NO_SOUND
        Mix_PlayChannel( -1, m_player_snds[PSND_SHOOTING], 0 );
#endif
    }
}
