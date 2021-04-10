/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Load.h
// For the loading screen

#ifndef LOAD_H
#define LOAD_H

class CLoad
{
    public:
        CLoad(SDL_Texture *background, SDL_Texture *circle )
        {
            m_background = background;
            m_circle = circle;
            m_num_circles = 0;

            int w, h;
            SDL_QueryTexture(circle, nullptr, nullptr, &w, &h);
            
            m_circle_rect.w = w;
            m_circle_rect.h = h;
            m_circle_rect.y = SCR_H - m_circle_rect.h;
        }
        
        void AddAndDraw()
        { AddCircle(); Draw(); }
        
        void AddCircle()
        {
            // Draw no more circles when we are out of the screen
            if( SCR_W / m_circle_rect.w > m_num_circles )
                m_num_circles++;
        }
        
        void Draw()
        {
            int i;
            // Draw background
            g_framework->Draw( m_background );
            
            // Draw circles
            for( i=0; i<m_num_circles; i++ )
            {
                m_circle_rect.x = i*m_circle_rect.w;
				if( m_circle_rect.x + m_circle_rect.w < SCR_W )
					g_framework->Draw( m_circle, &m_circle_rect );					
            }       
            
            SDL_RenderPresent( g_framework->GetRenderer() );
        }
    private:
        
        SDL_Texture *m_background, *m_circle;
        SDL_Rect m_circle_rect;
        
        int m_num_circles;
};

#endif
