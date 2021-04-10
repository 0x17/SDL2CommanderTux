/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Editor.h

#ifndef EDITOR_H
#define EDITOR_H

#include "State.h"
#include "Level.h"

struct Selection_Box
{
    int selected;
    int ypos;
    SDL_Rect rect;
};

// Modes

#define NUM_MODES   3

static char *mode_filenames[NUM_MODES] = {
    DATA_PREFIX "/images/editor/tile_mode.bmp",
    DATA_PREFIX "/images/editor/enemy_mode.bmp",
    DATA_PREFIX "/images/editor/item_mode.bmp"
};

enum
{
    MODE_TILE,
    MODE_ENEMY,
    MODE_ITEM
};

class CEditor : public CState
{
    public:
        CEditor();
        ~CEditor();
        
        virtual void Input();
        virtual void Update();
        virtual void Draw();
    private:
        CLevel *m_level;
        Selection_Box m_tile_box, m_enemy_box, m_item_box;
        SDL_Texture *m_selection_surf;
        SDL_Texture *m_tile_mode, *m_enemy_mode, *m_item_mode;
        int m_mouse_x, m_mouse_y;
        int m_scroll_x, m_scroll_y;
        SReleased m_released;
        int m_mode;
};

#endif
