/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Splash.h

#ifndef CSPLASH_H
#define CSPLASH_H

#include <SDL.h>
#include <SDL_image.h>
#include "State.h"

#define FADE_SPEED  5
#define ROLL_SPEED  10

class CSplash : public CState
{
    public:
        CSplash();
        virtual ~CSplash();
        
        virtual void Input();
        virtual void Update();
        virtual void Draw();
    private:
        SReleased m_released;
        SDL_Texture *m_splash_surf;
        SDL_Texture *m_version_surf;
        SDL_Rect m_splash_rect, m_version_rect;
        Uint8 m_splash_surf_fade;
        int m_splash_surf_roll;
#ifndef NO_SOUND
        Mix_Chunk *m_splash_sound;
#endif
};

#endif
