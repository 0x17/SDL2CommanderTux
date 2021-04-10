/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Text.cpp

#include "Text.h"

//
// CFlyingText
// Constructor
//
CFlyingText::CFlyingText(	const char *text,
							int s_pos_x, int s_pos_y,
							Direction dir, Uint32 duration,
							Color color )
{
    // Copy func parameters to member vars
    m_dir = dir;
    m_duration = duration;
    
    m_rect.x = s_pos_x;
    m_rect.y = s_pos_y;
    
    // Create surface
    m_surface = g_framework->DrawTextOnSurface( text,
                                                g_framework->GetColorSDLColor( color ),
                                                F_SIZE_MEDIUM );
    m_rect.w = g_framework->GetTextureWidth(m_surface);
    m_rect.h = g_framework->GetTextureHeight(m_surface);
    
    // Init start time
    m_start = g_framework->GetTicks();
}

//
// ~CFlyingText
// Destructor
//
CFlyingText::~CFlyingText()
{
    // Free surface
    SDL_DestroyTexture( m_surface );
}

//
// Update
// Returns whether still alive
//
bool CFlyingText::Update( int *scroll_x, int *scroll_y )
{
    // Is time not exceeded?
    if( g_framework->GetTicks() - m_start < m_duration )
    {
        // Only draw when still in the screen
        if( m_rect.y - *scroll_y - FLYING_SPEED > 0  &&
            m_rect.y - *scroll_y + m_rect.h < SCR_H &&
            m_rect.x - *scroll_x > 0 &&
            m_rect.x - *scroll_x + m_rect.w < SCR_W )
        {
            switch( m_dir )
            {
                case DIR_LEFT:
                    m_rect.x-=FLYING_SPEED;
                    break;
                case DIR_RIGHT:
                    m_rect.x+=FLYING_SPEED;
                    break;
                case DIR_UP:
                    m_rect.y-=FLYING_SPEED;
                    break;
                case DIR_DOWN:
                    m_rect.y+=FLYING_SPEED;
                    break;
            }
            
            return true;
        }
        else // Otherwise kill it
            return false;
    }
    else
        return false;
}

//
// Draw
//
void CFlyingText::Draw( int *scroll_x, int *scroll_y )
{
    g_framework->Draw( m_surface, m_rect.x - *scroll_x, m_rect.y - *scroll_y );
}
