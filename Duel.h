/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Duel.h

#ifndef DUEL_H
#define DUEL_H

#include "State.h"
#include "Level.h"

class CDuel : public CState 
{
    public:
        CDuel();
        virtual ~CDuel();
        
        virtual void Input();
        virtual void Update();
        virtual void Draw();
                
    private:
        CLevel *m_level;
        SReleased m_released[MAX_PLAYERS];
        int m_scroll_x, m_scroll_y;
#ifndef NO_SOUND
        Mix_Music *m_music;
#endif
};

#endif
