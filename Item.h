/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Item.h

#ifndef ITEM_H
#define ITEM_H

#include "Sprite.h"
#include "Defines.h"

static int item_scores[NUM_ITEMS] =
{
    5,  // Fish
    15, // Burger
    10, // Hotdog
    20, // C book
    25, // C++ book
    15, // SDL book
    10, // Drink
    50, // Nine Inch Nails CD
    0,  // Bullets
    0,  // Live    
};

class CItem : public CSprite
{
    public:
        CItem( SDL_Texture *image, int id )
        : CSprite( image ), m_id( id )
        { m_score = item_scores[id]; }
        
        void SetScore( int score ) { m_score = score; }
        int GetScore() { return m_score; }
        
        int GetId() { return m_id; }
        
    private:
        int m_score;
        int m_id;
};

#endif
