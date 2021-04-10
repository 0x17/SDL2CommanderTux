/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Text.h

#ifndef TEXT_H
#define TEXT_H

#include "Framework.h"

#define FLYING_SPEED 2

class CFlyingText
{
    public:
        CFlyingText(	const char *text,
						int s_pos_x, int s_pos_y,
						Direction dir, Uint32 duration, Color color );
        ~CFlyingText();
        
        bool Update( int *scroll_x, int *scroll_y );
        void Draw( int *scroll_x, int *scroll_y );
        
    private:
        
        SDL_Rect m_rect;
        SDL_Texture *m_surface;
        
        Direction m_dir;
        Uint32 m_duration, m_start;
};

#endif
