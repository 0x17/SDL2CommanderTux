/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include "Framework.h"

class CSprite
{
    public:
        CSprite( SDL_Texture *image = NULL);

        void SetImage( SDL_Texture *image );
        void SetPos( int x, int y );
        SDL_Rect *GetRect();
        void Draw();
        void Draw( int *scroll_x, int *scroll_y );
    protected:
        SDL_Texture *m_image;
        SDL_Rect m_rect;
};

class CAnim_Sprite : public CSprite
{
    public:
        CAnim_Sprite(	SDL_Texture *image,
						int image_w, int image_h,
                        Uint32 chg_time );
        
        void SetActiveImage( int nr ) { m_act_image = nr; }
        int GetActImage() { return m_act_image; }
        void Iterate();
        
        virtual void Draw( bool iterate );
    private:
        SDL_Rect m_src_rect;
        int m_act_image;
        Uint32 m_last_change;        
        Uint32 m_chg_time;
        int m_num_images;
};

#endif
