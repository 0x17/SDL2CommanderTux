/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Duel.cpp

#include "Duel.h"

//
// CDuel
// Constructor
//
CDuel::CDuel() : CState()
{
    DEBUG( "Setting up duel state... " );
    
    // In duel mode we don't scroll, so we have to set the scroll value
    // right from the beginning. As usual this is in the lower left corner.
    m_scroll_x = 0;
    m_scroll_y = 44*TILE_H;
    
    // There's only one duel level
    m_level = new CLevel( duel_lvl_filenames[DUEL_LVL_1], false, true );
    
#ifndef NO_SOUND
    /*// Only one music for this game mode too
    m_music = Mix_LoadMUS("data/music/heavy.wav");
    Mix_PlayMusic( m_music, -1 );*/
#endif
    
    DEBUG( "Duel state is set up!" );
}

//
// CDuel
// Destructor
//
CDuel::~CDuel()
{
    DEBUG( "Shutting down the duel state... ");
    
    // Delete level
    if( m_level )
        delete m_level;
#ifndef NO_SOUND
    // Free music
    /*
    Mix_FreeMusic( m_music );*/
#endif
    
    DEBUG( "Duel state is shut down!" );
}

//
// Input
// Duel state specific input processing
//
void CDuel::Input()
{
    // Standard stuff
    CState::Input();
    
    // Standard input
    CState::Input();
    if( m_keys[SDL_SCANCODE_ESCAPE] )
        m_done = true;
    
    // Player deactivate / activate
    
    // Player 2 
    if( m_keys[SDL_SCANCODE_2] )
    {
        if( m_released[0].key_2 )
        {
            if( m_level->GetPlayer(1)->GetActive() )
                m_level->GetPlayer(1)->SetActive( false );
            else
                m_level->GetPlayer(1)->SetActive( true );
            m_released[0].key_2 = false;
        }
    }
    else
        m_released[0].key_2 = true;
    
    // Player 3 
    if( m_keys[SDL_SCANCODE_3] )
    {
        if( m_released[0].key_3 )
        {
            if( m_level->GetPlayer(2)->GetActive() )
                m_level->GetPlayer(2)->SetActive( false );
            else
                m_level->GetPlayer(2)->SetActive( true );
            m_released[0].key_3 = false;
        }
    }
    else
        m_released[0].key_3 = true;
    
    // Player 4 
    if( m_keys[SDL_SCANCODE_4] )
    {
        if( m_released[0].key_4 )
        {
            if( m_level->GetPlayer(3)->GetActive() )
                m_level->GetPlayer(3)->SetActive( false );
            else
                m_level->GetPlayer(3)->SetActive( true );
            m_released[0].key_4 = false;
        }
    }
    else
        m_released[0].key_4 = true;
    
    // Player 1 - left, right, up
    
    // Walking    
    if( m_keys[SDL_SCANCODE_LEFT] )
        m_level->GetPlayer()->Walk( DIR_LEFT );
        
    if( m_keys[SDL_SCANCODE_RIGHT] )
        m_level->GetPlayer()->Walk( DIR_RIGHT );
        
    // Jumping        
    if( m_keys[SDL_SCANCODE_UP] )
    {
        if( m_released[0].key_return )
        {
            m_level->GetPlayer()->Jump();
            m_released[0].key_return = false;
        }
        else
            m_level->GetPlayer()->IncreaseJump();
    }
    else
        m_released[0].key_return = true;
    
    // Standing
    if( !m_keys[SDL_SCANCODE_LEFT] && !m_keys[SDL_SCANCODE_RIGHT] )
        m_level->GetPlayer()->SetStanding( true );
    
    // Player 2 - a, d, w
    
    if( m_level->GetPlayer(1)->GetActive() )
    {
    
        // Walking    
        if( m_keys[SDL_SCANCODE_A] )
            m_level->GetPlayer(1)->Walk( DIR_LEFT );
            
        if( m_keys[SDL_SCANCODE_D] )
            m_level->GetPlayer(1)->Walk( DIR_RIGHT );
        
        // Jumping        
        if( m_keys[SDL_SCANCODE_W] )
        {
            if( m_released[1].key_return )
            {
                m_level->GetPlayer(1)->Jump();
                m_released[1].key_return = false;
            }
            else
                m_level->GetPlayer(1)->IncreaseJump();
        }
        else
            m_released[1].key_return = true;
        
        // Standing
        if( !m_keys[SDL_SCANCODE_A] && !m_keys[SDL_SCANCODE_D] )
            m_level->GetPlayer(1)->SetStanding( true );
        
    } // Player 2 active
    
    // Player 3 - f, h, t
    
    if( m_level->GetPlayer(2)->GetActive() )
    {
            
        // Walking    
        if( m_keys[SDL_SCANCODE_F] )
            m_level->GetPlayer(2)->Walk( DIR_LEFT );
            
        if( m_keys[SDL_SCANCODE_H] )
            m_level->GetPlayer(2)->Walk( DIR_RIGHT );
        
        // Jumping        
        if( m_keys[SDL_SCANCODE_T] )
        {
            if( m_released[2].key_return )
        {
                m_level->GetPlayer(2)->Jump();
                m_released[2].key_return = false;
        }
            else
                m_level->GetPlayer(2)->IncreaseJump();
        }
        else
            m_released[2].key_return = true;
        
        // Standing
    if( !m_keys[SDL_SCANCODE_F] && !m_keys[SDL_SCANCODE_H] )
            m_level->GetPlayer(2)->SetStanding( true );
        
    } // Player 3 active
    
    // Player 4 - j, l, i
    
    if( m_level->GetPlayer(3)->GetActive() )
    {
    
        // Walking    
        if( m_keys[SDL_SCANCODE_J] )
            m_level->GetPlayer(3)->Walk( DIR_LEFT );
            
        if( m_keys[SDL_SCANCODE_L] )
            m_level->GetPlayer(3)->Walk( DIR_RIGHT );
            
        // Jumping        
        if( m_keys[SDL_SCANCODE_I] )
        {
            if( m_released[3].key_return )
            {
                m_level->GetPlayer(3)->Jump();
                m_released[3].key_return = false;
            }
            else
                m_level->GetPlayer(3)->IncreaseJump();
        }
        else
            m_released[3].key_return = true;
    
        // Standing
        if( !m_keys[SDL_SCANCODE_J] && !m_keys[SDL_SCANCODE_L] )
            m_level->GetPlayer(3)->SetStanding( true );
         
    } // Player 4 active
}

//
// Update
// Duel state specific game logic
//
void CDuel::Update()
{
    m_level->Update ( &m_scroll_x, &m_scroll_y );
}

//
// Draw
// Duel state specific drawing routine
//
void CDuel::Draw()
{
    m_level->Draw( &m_scroll_x, &m_scroll_y, SCR_W, SCR_H, false );
}
