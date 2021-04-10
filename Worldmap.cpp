/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Worldmap.cpp

#include "Worldmap.h"

//
// CWorldmap
// Constructor
//
CWorldmap::CWorldmap()
{
    int i;
    
    // Copy params
    m_scroll_x = 500;
    m_scroll_y = 200;
    m_flip = false;
    m_last_flip = g_framework->GetTicks();
    m_walking = false;
    m_player_angle = 135;
    m_game_running = false;
    m_player_lives = NUM_LIVES;
    m_player_score = m_player_bullets = 0;

    // Init player rect
    m_player_rect.x = 620;
    m_player_rect.y = 230;
    
    // Set default values
    m_world_src_rect.x = m_scroll_x;
    m_world_src_rect.y = m_scroll_y;
    m_world_src_rect.w = SCR_W;
    m_world_src_rect.h = SCR_H;  
    
    // Level stuff
    
    // Load level done image
    m_level_done_surface = g_framework->LoadImage( worldmap_level_done_filename[LVL_DONE] );
    
    // Set the right colorkey
    g_framework->SetColorKey( m_level_done_surface, COLOR_MAGENTA );
    
    for( i=0; i<NUM_LEVELS; i++ )
    {
        // Set level unfinished -> not done!
        m_level_done[i] = false;
        
        // Load level image from the disk
        m_level_surfaces[i] = g_framework->LoadImage( worldmap_level_filenames[i] );
        
        // Set the right colorkey
        g_framework->SetColorKey( m_level_surfaces[i], COLOR_MAGENTA );
        
        // Init the rectangle sizes
        m_level_rectangles[i].w = g_framework->GetTextureWidth(m_level_surfaces[i]);
        m_level_rectangles[i].h = g_framework->GetTextureHeight(m_level_surfaces[i]);
    }
    
    // Set level image positions using their rectangles  
    
    // Level 1
    m_level_rectangles[0].x = 850;
    m_level_rectangles[0].y = 240;
    
    // Level 2
    m_level_rectangles[1].x = 790;
    m_level_rectangles[1].y = 431;
    
    // Level 3
    m_level_rectangles[2].x = 649;
    m_level_rectangles[2].y = 610;
    
    // Level 4
    m_level_rectangles[3].x = 634;
    m_level_rectangles[3].y = 831;
        
    // Init walkable areas
    
    // Area 1
    m_walkable_areas[0].x = 619;
    m_walkable_areas[0].y = 227;
    m_walkable_areas[0].w = 321;
    m_walkable_areas[0].h = 590;
    
    // Area 2
    m_walkable_areas[1].x = 629;
    m_walkable_areas[1].y = 821;
    m_walkable_areas[1].w = 67;
    m_walkable_areas[1].h = 201;    
    
    // Area 3
    m_walkable_areas[2].x = 426;
    m_walkable_areas[2].y = 1027;
    m_walkable_areas[2].w = 609;
    m_walkable_areas[2].h = 323;    
}

//
// CWorldmap
// Destructor
//
CWorldmap::~CWorldmap()
{
    int i;
    
    // Free level images
    for( i=0; i<NUM_LEVELS; i++ )
        SDL_DestroyTexture( m_level_surfaces[i] );
    SDL_DestroyTexture( m_level_done_surface );
}

//
// SetImages
//
void CWorldmap::SetImages(	SDL_Texture *world,
							SDL_Texture *player[2][NUM_W_P_IMAGES] )
{
	int i, j;

	m_world = world;

	// Copy parameter surfaces to member surf.
    for( i=0; i<NUM_W_P_IMAGES; i++ )
    {
        for( j=0; j<2; j++ )
            m_player[j][i] = player[j][i];
    }

	// Player rect
	m_player_rect.w = g_framework->GetTextureWidth(m_player[0][0]);
    m_player_rect.h = g_framework->GetTextureHeight(m_player[0][0]);
}

//
// Input
//
void CWorldmap::Input(const Uint8 *keys )
{
    int i;
    
    // Rotate player left // decrement angle
    if( keys[SDL_SCANCODE_LEFT] )
    {
        if( m_released.key_left )
        {
            if( m_player_angle > 0 )
                m_player_angle-=45;
            else
                m_player_angle = 315;
        
            m_released.key_left = false;
        }
    }
    else
        m_released.key_left = true;
    
    // Rotate player right // increment angle
    if( keys[SDL_SCANCODE_RIGHT] )
    {
        if( m_released.key_right )
        {
            if( m_player_angle < 315 )
                m_player_angle+=45;
            else
                m_player_angle = 0;
                
            m_released.key_right = false;
        }
    }
    else
        m_released.key_right = true;
        
    // Walk forward
    
    int straight_speed = (Sint16)sqrt((float)(2*(WORLD_WALK_SPEED*WORLD_WALK_SPEED)))/2;
    
    if( keys[SDL_SCANCODE_UP] )
    {
        m_walking = true;
        
        switch( m_player_angle )
        {
            case 0:
                // North
                if( WalkPossible( 0, -straight_speed ) )
                    m_player_rect.y-=straight_speed;
                break;
            case 45:
                // North East
                if( WalkPossible( WORLD_WALK_SPEED/2, -WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y-=WORLD_WALK_SPEED/2;
                    m_player_rect.x+=WORLD_WALK_SPEED/2;
                }
                break;
            case 90:
                // East
                if( WalkPossible( straight_speed, 0 ) )
                    m_player_rect.x+=straight_speed;
                break;
            case 135:
                // South East
                if( WalkPossible( WORLD_WALK_SPEED/2, WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y+=WORLD_WALK_SPEED/2;
                    m_player_rect.x+=WORLD_WALK_SPEED/2;
                }
                break;
            case 180:
                // South
                if( WalkPossible( 0, straight_speed ) )
                    m_player_rect.y+=straight_speed;
                break;
            case 225:
                // South West
                if( WalkPossible( -WORLD_WALK_SPEED/2, WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y+=WORLD_WALK_SPEED/2;
                    m_player_rect.x-=WORLD_WALK_SPEED/2;
                }
                break;
            case 270:
                // West
                if( WalkPossible( -straight_speed, 0 ) )
                    m_player_rect.x-=straight_speed;
                break;
            case 315:
                // North West
                if( WalkPossible( -WORLD_WALK_SPEED/2, -WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y-=WORLD_WALK_SPEED/2;
                    m_player_rect.x-=WORLD_WALK_SPEED/2;
                }
                break;
        }
    }
        
    // Walk backward
    if( keys[SDL_SCANCODE_DOWN] )
    {
        m_walking = true;
        
        switch( m_player_angle )
        {
            case 0:
                // North
                if( WalkPossible( 0, straight_speed ) )
                    m_player_rect.y+=straight_speed;
                break;
            case 45:
                // North East
                if( WalkPossible( -WORLD_WALK_SPEED/2, WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y+=WORLD_WALK_SPEED/2;
                    m_player_rect.x-=WORLD_WALK_SPEED/2;
                }
                break;
            case 90:
                // East
                if( WalkPossible( -straight_speed, 0 ) )
                    m_player_rect.x-=straight_speed;
                break;
            case 135:
                // South East
                if( WalkPossible( -WORLD_WALK_SPEED, -WORLD_WALK_SPEED ) )
                {
                    m_player_rect.y-=WORLD_WALK_SPEED/2;
                    m_player_rect.x-=WORLD_WALK_SPEED/2;
                }
                break;
            case 180:
                // South
                if( WalkPossible( 0, -straight_speed ) )
                    m_player_rect.y-=straight_speed;
                break;
            case 225:
                // South West
                if( WalkPossible( WORLD_WALK_SPEED/2, -WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y-=WORLD_WALK_SPEED/2;
                    m_player_rect.x+=WORLD_WALK_SPEED/2;
                }
                break;
            case 270:
                // West
                if( WalkPossible( straight_speed, 0 ) )
                    m_player_rect.x+=straight_speed;
                break;
            case 315:
                // North West
                if( WalkPossible( WORLD_WALK_SPEED/2, WORLD_WALK_SPEED/2 ) )
                {
                    m_player_rect.y+=WORLD_WALK_SPEED/2;
                    m_player_rect.x+=WORLD_WALK_SPEED/2;
                }
                break;
        }
    }
    
    // Standing
    if( !keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN] )
    {
        m_walking = false;
        m_player_state = STANDING;
    }
    
    // Return pressed
    // Trying to enter a level
    if( keys[SDL_SCANCODE_RETURN] )
    {
        if( m_released.key_return )
        {
            // Collision with representation of a level?
            for( i=0; i<NUM_LEVELS; i++ )
            {
                if( g_framework->Coll( &m_player_rect, &m_level_rectangles[i] ) )
                {
                    SetActLevel( i );
                    SetGameRunning( true );
                }
            }
                        
            m_released.key_return = false;
        }
    }
    else
        m_released.key_return = true;
}

//
// Update
//
void CWorldmap::Update()
{
    int i;
    
    // Scrolling
    for( i=0; i<WORLD_SCROLL_SPEED; i++ )
    {
        // Left
        if( m_player_rect.x - m_scroll_x < WORLD_PLAYER_RECT_X 
            && m_scroll_x > 0 )
            m_scroll_x--;
        // Right
        if( m_player_rect.x + m_player_rect.w - m_scroll_x > WORLD_PLAYER_RECT_X + WORLD_PLAYER_RECT_W
            && m_scroll_x + SCR_W + 1 < g_framework->GetTextureWidth(m_world) )
            m_scroll_x++;
        // Up
        if( m_player_rect.y - m_scroll_y < WORLD_PLAYER_RECT_Y
            && m_scroll_y > 0 )
            m_scroll_y--;
        // Down        
        if( m_player_rect.y + m_player_rect.h - m_scroll_y > WORLD_PLAYER_RECT_Y + WORLD_PLAYER_RECT_H
            && m_scroll_y + SCR_H < g_framework->GetTextureHeight(m_world))
            m_scroll_y++;
    }
    
    
    // Update rectangles
    
    // World rect
    m_world_src_rect.x = m_scroll_x;
    m_world_src_rect.y = m_scroll_y;
    
    // Flipping / for walking
    if( g_framework->GetTicks() - m_last_flip > WORLD_P_IMG_FLIP_TIME && m_walking )
    {
        // Reverse flip
        g_framework->ReverseBool( &m_flip );
        
        // Set image
        if( m_flip )
            m_player_state = WALKING;
        else
            m_player_state = STANDING;
        
        // Update last_flip time
        m_last_flip = g_framework->GetTicks();
    }
}

//
// Draw
//
void CWorldmap::Draw()
{
    int i;
    
    // Draw the worldmap
    g_framework->Draw( m_world, &m_world_src_rect, NULL );
    
    // Draw level images    
    for( i=0; i<NUM_LEVELS; i++ )
    {        
        g_framework->Draw(  m_level_surfaces[i],
                            m_level_rectangles[i].x - m_scroll_x,
                            m_level_rectangles[i].y - m_scroll_y  );
        
        // Draw level done image where needed
        if( m_level_done[i] )
            g_framework->Draw(  m_level_done_surface,
                                m_level_rectangles[i].x - m_scroll_x,
                                m_level_rectangles[i].y - m_scroll_y  );
    }    
    
    // Draw player    
    g_framework->Draw(  m_player[m_player_state][m_player_angle/45],
                        m_player_rect.x - m_scroll_x,
                        m_player_rect.y - m_scroll_y  );
}

//
// WalkPossible
//
bool CWorldmap::WalkPossible( int x, int y )
{
    int i;
    
    // New rect for desired position
    static SDL_Rect new_player_rect;
    memcpy( &new_player_rect, &m_player_rect, sizeof( SDL_Rect ) );
    
    new_player_rect.x+=x;
    new_player_rect.y+=y;
    
    // Check if the player is still in the walkable area with the new position
    for( i=0; i<NUM_WALKABLE_AREAS; i++ )
    {
        if( i==0 || m_level_done[3] ) // only go outside of rectangle1 if level 4 is done
        {
            // Collision between player and walkable areas
            if( g_framework->Coll( &m_walkable_areas[i], &new_player_rect ) )
                return true;
        }
    }
    
    return false;
}
