/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// SLevel.h

#ifndef SLEVEL_H
#define SLEVEL_H

#include "Defines.h"

// Level structure. Holds all the data saved in the *.ctux level files.
struct SLevel
{
    int content[LVL_H_MAX][LVL_W_MAX];
        
    SLevel()
    {
        int i, j;        
        for( i=0; i<LVL_H_MAX; i++ )
        {
            for( j=0; j<LVL_W_MAX; j++ )
                content[i][j] = TILE_EMPTY;
        }
    }
};

#endif
