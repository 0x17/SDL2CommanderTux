/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Splash.cpp

#include "Splash.h"

//
// CSplash
// Constructor
//
CSplash::CSplash() : CState()
{
    DEBUG( "Setting up the splashscreen..." );
    
    // Set maximal framerate per second
    m_max_fps = 80;

    // Load splash surface
    m_splash_surf = g_framework->LoadImage( splash_filenames[SPL_SPLASH] );

#ifndef NO_SOUND
    // Load splash sound and play it
    m_splash_sound = Mix_LoadWAV( splash_filenames[SPL_SOUND] );
    Mix_PlayChannel( -1, m_splash_sound, 0 );
#endif

    int m_splash_surf_w = g_framework->GetTextureWidth(m_splash_surf);
    int m_splash_surf_h = g_framework->GetTextureHeight(m_splash_surf);
    
    // Init splash surface rectangle for fade in
    m_splash_rect.w = m_splash_surf_w;
    m_splash_rect.h = m_splash_surf_h;
    m_splash_rect.x = 0;
    m_splash_surf_roll = -m_splash_surf_h;
    
    // Init var for fading
    m_splash_surf_fade = 0;
    
    // Load version surface based upon text drawed on surface by the framework
    m_version_surf = g_framework->DrawTextOnSurface( VERSION_C, g_framework->GetColorSDLColor( COLOR_GREEN ),F_SIZE_MEDIUM );

    DEBUG( "Splashscreen is working!" );
}

//
// CSplash
// Destructor
//
CSplash::~CSplash()
{
    DEBUG( "Shutting down the splashscreen..." );
    
#ifndef NO_SOUND
    // Delete splash sound
    Mix_FreeChunk( m_splash_sound );
#endif

    // Free version surface
    SDL_DestroyTexture( m_version_surf );
    // Free splash surface
    SDL_DestroyTexture( m_splash_surf );
    
    DEBUG( "Splashscreen is shutdown!" );
}

//
// Input
// Splash specific input processing.
//
void CSplash::Input()
{
    // Standard state input
    CState::Input();
    
    // Get to the game state when esc or return is pressed
    
    // Check wheter escape was released because you will jump in here
    // when you exit the editor or duel mode
    if( m_keys[SDL_SCANCODE_ESCAPE] )
    {
        if( m_released.key_escape )
        {
            m_released.key_escape = false;
            m_done = true;
        }
    }
    else
        m_released.key_escape = true;
    
    if( m_keys[SDL_SCANCODE_RETURN] )
        m_done = true;
}

//
// Update
// Splash specific update function.
//
void CSplash::Update()
{
    // Fade if neccessary
    if( m_splash_surf_fade < SDL_ALPHA_OPAQUE - FADE_SPEED )
        m_splash_surf_fade += FADE_SPEED;
    if( m_splash_surf_roll < 0 )
    {
        m_splash_surf_roll += ROLL_SPEED;
        m_splash_rect.y = m_splash_surf_roll;
    }
}

//
// Draw
// Splash specific drawing stuff.
//
void CSplash::Draw()
{
    // Because this is too slow in fullscreen
    // don't fade...
    if( !g_framework->GetSetup()->fullscreen )
    {
        g_framework->FillRect( g_framework->GetScreen(),
                            NULL, COLOR_BLACK );
                            
        g_framework->SetAlpha( m_splash_surf, m_splash_surf_fade );
        // We want to draw outside the screen so we can't use any
        // drawing function of the framework
        SDL_RenderCopy(g_framework->GetRenderer(), m_splash_surf, nullptr, &m_splash_rect );
    }
    else
        g_framework->Draw( m_splash_surf );    
    
    // Draw version
    g_framework->Draw( m_version_surf, 10, SCR_H - 20 );
}
