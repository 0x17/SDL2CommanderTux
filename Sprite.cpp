/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Sprite.cpp

#include "Sprite.h"

//
// CSprite
// Constructor
//
CSprite::CSprite( SDL_Texture *image )
{
    SetImage( image );
    m_rect.x = m_rect.y = 0;
}

//
// CSprite
// Destructor
//
void CSprite::SetImage( SDL_Texture *image )
{
    if( image )
    {
        m_image = image;

        int w, h;
        SDL_QueryTexture(image, nullptr, nullptr, &w, &h);

        m_rect.w = w;
        m_rect.h = h;
    }
}

//
// SetPos
//
void CSprite::SetPos( int x, int y )
{
    m_rect.x = x;
    m_rect.y = y;
}

//
// GetRect
//
SDL_Rect *CSprite::GetRect()
{
    return &m_rect;
}

//
// Draw
//
void CSprite::Draw()
{
    g_framework->Draw( m_image, &m_rect );
}

//
// Draw
//
void CSprite::Draw( int *scroll_x, int *scroll_y )
{
    // Draw the image at the right position
    g_framework->Draw( m_image,
                       m_rect.x - *scroll_x,
                       m_rect.y - *scroll_y );
}

//
// CAnim_Sprite
// Constructor
//
CAnim_Sprite::CAnim_Sprite( SDL_Texture *image,
							int image_w, int image_h,
							Uint32 chg_time )
: CSprite( image )
{
    // Copy parameter values to member vars
    m_chg_time = chg_time;
    
    m_num_images = m_rect.w / image_w;
    m_src_rect.w = image_w;
    m_src_rect.h = image_h;
    m_act_image = 0;
    m_src_rect.x = m_act_image * image_w;
    m_src_rect.y = 0;
    m_last_change = g_framework->GetTicks();    
}

//
// Draw
//
void CAnim_Sprite::Draw( bool iterate)
{
    if( iterate )
        Iterate();
    m_src_rect.x = m_act_image * m_src_rect.w;

    m_rect.w = m_src_rect.w;
    m_rect.h = m_src_rect.h;

    g_framework->Draw( m_image, &m_src_rect, &m_rect );
}

//
// Iterate
//
void CAnim_Sprite::Iterate()
{
    if( g_framework->GetTicks() - m_last_change > m_chg_time )
    {
        if( m_act_image < m_num_images -1 )
            m_act_image++;
        else
            m_act_image = 0;
        
        m_last_change = g_framework->GetTicks();
    }    
}
