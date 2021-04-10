/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Editor.cpp

#include "Editor.h"

//
// CEditor
// Constructor
//
CEditor::CEditor() : CState()
{
    DEBUG( "Editor is starting... " );
    
    // Video stuff
    g_framework->ShowCursor( true );
    g_framework->SetVideoMode( EDIT_W, EDIT_H );
    
    // Allocate new level from scratch
    m_level = NULL;
    m_level = new CLevel( DATA_PREFIX "/levels/level3.ctux", true, false );
    //m_level = new CLevel( NULL, true, false );
    
    // Tile box
    m_tile_box.selected = 0;
    m_tile_box.ypos = EDIT_H - ( TILE_H + 10 ) - 110;
    // Enemy box
    m_enemy_box.selected = 0;
    m_enemy_box.ypos = EDIT_H - ( TILE_H + 10 ) - 70;
    // Item box
    m_item_box.selected = 0;
    m_item_box.ypos = EDIT_H - ( TILE_H + 10 ) - 30;
    
    // Selection surface
    m_selection_surf =
        g_framework->LoadImage( editor_filenames[ ED_TILE_SELECTED ] );
        
    g_framework->SetColorKey( m_selection_surf, COLOR_MAGENTA );
    // Scroll vars
    m_scroll_x = 0;
    m_scroll_y = (LVL_H_MAX*TILE_H)-(20*TILE_H);
    
    // Selected mode
    m_tile_mode = g_framework->LoadImage( mode_filenames[MODE_TILE] );
    m_enemy_mode = g_framework->LoadImage( mode_filenames[MODE_ENEMY] );
    m_item_mode = g_framework->LoadImage( mode_filenames[MODE_ITEM] );
    
    m_mode = MODE_TILE;
    
    DEBUG( "Editor started!" );
}

//
// CEditor
// Destructor
//
CEditor::~CEditor()
{
    DEBUG( "Shutting down the editor... ");
    
    // Free surfaces
    SDL_DestroyTexture( m_item_mode );
    SDL_DestroyTexture( m_enemy_mode );
    SDL_DestroyTexture( m_tile_mode );
    SDL_DestroyTexture( m_selection_surf );
    
    // Delete the level
    delete m_level;
    
    // Reset to old video mode
    g_framework->SetVideoMode();
    
    // Set cursor deactive for other modes
    g_framework->ShowCursor( false );
    
    DEBUG( "Editor shut down!");
}

//
// Input
// Editor specific input.
//
void CEditor::Input()
{
    static SDL_Rect temp_rect, temp_rect2;
    
    // Standard input
    CState::Input();
    if( m_keys[SDL_SCANCODE_ESCAPE] )
        m_done = true;
    
    // MOUSE INPUT
    
    if( SDL_GetMouseState( &m_mouse_x, &m_mouse_y ) & SDL_BUTTON( 1 ) )
    {
        // For coll detection
        temp_rect2.w = 10; temp_rect2.h = 10;
        temp_rect2.x = m_mouse_x;
        temp_rect2.y = m_mouse_y;
        
        temp_rect.w = TILE_W; temp_rect.h = TILE_H;        
        
        // Click into the grid
        if( m_mouse_x < 900 && m_mouse_x > 0 &&
            m_mouse_y < 600 && m_mouse_y > 0 )
            {
                if( m_mode == MODE_TILE ) // Tile mode
                {
                    
                    if( m_tile_box.selected < NUM_TILES )
                    {
                    m_level->GetLevelStruct()->content
                    [(m_mouse_y+m_scroll_y)/TILE_H][(m_mouse_x+m_scroll_x)/TILE_W]
                     = m_tile_box.selected;
                    }
                    else
                        m_level->GetLevelStruct()->content[(m_mouse_y+m_scroll_y)/
                        TILE_H][(m_mouse_x+m_scroll_x)/TILE_W] = TILE_EMPTY;
                }
                else if( m_mode == MODE_ENEMY ) // Enemy mode
                {
                    if( m_enemy_box.selected < NUM_ENEMIES )
                    {
                    m_level->GetLevelStruct()->content
                    [(m_mouse_y+m_scroll_y)/TILE_H][(m_mouse_x+m_scroll_x)/TILE_W]
                     = m_enemy_box.selected+ENEMY_SHIFT;
                    }
                    else
                        m_level->GetLevelStruct()->content[(m_mouse_y+m_scroll_y)/
                        TILE_H][(m_mouse_x+m_scroll_x)/TILE_W] = TILE_EMPTY;
                    
                }
                else // Item mode
                {
                    if( m_item_box.selected < NUM_ITEMS )
                    {
                    m_level->GetLevelStruct()->content
                    [(m_mouse_y+m_scroll_y)/TILE_H][(m_mouse_x+m_scroll_x)/TILE_W]
                     = m_item_box.selected+ITEM_SHIFT;
                    }
                    else
                        m_level->GetLevelStruct()->content[(m_mouse_y+m_scroll_y)/
                        TILE_H][(m_mouse_x+m_scroll_x)/TILE_W] = TILE_EMPTY;
                }
            }
        // Outside the grid
        else if( m_mouse_x > 0 && m_mouse_x < EDIT_W &&
                m_mouse_y > m_tile_box.ypos &&
                m_mouse_y < m_tile_box.ypos + TILE_H ) // Tile box
        {
            m_tile_box.selected = m_mouse_x / TILE_W;
        }
        else if( m_mouse_x > 0 && m_mouse_x < EDIT_W &&
                m_mouse_y > m_enemy_box.ypos &&
                m_mouse_y < m_enemy_box.ypos + TILE_H ) // Enemy box
        {
            m_enemy_box.selected = m_mouse_x / TILE_W;
        }
        else if( m_mouse_x > 0 && m_mouse_x < EDIT_W &&
                m_mouse_y > m_item_box.ypos &&
                m_mouse_y < m_item_box.ypos + TILE_H ) // Item box
        {
            m_item_box.selected = m_mouse_x / TILE_W;
        }        
        else // Mode selection
        {
            temp_rect.x = EDIT_W - 40;
            temp_rect.y = EDIT_H - 40;
            // Tile mode
            if( g_framework->Coll( &temp_rect, &temp_rect2 ) )
            {
                m_mode = MODE_TILE;
            }
            
            // Enemy mode
            temp_rect.x = EDIT_W - 80;
            if( g_framework->Coll( &temp_rect, &temp_rect2 ) )
            {
                m_mode = MODE_ENEMY;
            }
            
            // Item mode
            temp_rect.x = EDIT_W - 120;
            if( g_framework->Coll( &temp_rect, &temp_rect2 ) )
            {
                m_mode = MODE_ITEM;
            }
        }
    }
    
    // SCROLLING
    
    if( m_keys[SDL_SCANCODE_RIGHT] )
    {
        if( m_released.key_right )
        {
            if( m_scroll_x/TILE_W + 30 < LVL_W_MAX )
            {
                m_scroll_x+=TILE_W;
                m_released.key_right = false;
            }
        }
    }
    else
        m_released.key_right = true;
    
    if( m_keys[SDL_SCANCODE_LEFT] )
    {
        if( m_released.key_left )
        {
            if( m_scroll_x-TILE_W >= 0 )
            {
                m_scroll_x-=TILE_W;
                m_released.key_left = false;
            }
        }
    }
    else
        m_released.key_left = true;
    
    if( m_keys[SDL_SCANCODE_DOWN] )
    {
        if( m_released.key_down )
        {
            if( m_scroll_y/TILE_H + 20 < LVL_H_MAX )
            {
                m_scroll_y+=TILE_H;
                m_released.key_down = false;
            }
        }
    }
    else
        m_released.key_down = true;
    
    if( m_keys[SDL_SCANCODE_UP] )
    {
        if( m_released.key_up )
        {
            if( m_scroll_y-TILE_H >= 0 )
            {
                m_scroll_y-=TILE_H;
                m_released.key_up = false;
            }
        }
    }
    else
        m_released.key_up = true;
    
    // Special features
    
    if( m_keys[SDL_SCANCODE_S] )
    {
        if( m_released.key_s )
        {
            m_level->Save( DATA_PREFIX "/levels/unnamed.ctux" );
            m_released.key_s = false;
        }
    }
    else
        m_released.key_s = true;
    
    if( m_keys[SDL_SCANCODE_P] )
    {
        if( m_released.key_p )
        {
            m_level->PrintLevelContent();
            m_released.key_p = false;
        }
    }
    else
        m_released.key_p = true;
}

//
// Update
// Editor specific update.
//
void CEditor::Update()
{
    // Empty
}

//
// Draw
// Editor specific draw func.
//
void CEditor::Draw()
{
    int i;

    SDL_RenderClear(g_framework->GetRenderer());
    
    // Clear
    g_framework->FillRect( g_framework->GetScreen(), NULL, COLOR_BLACK );
    
    // Lvl
    m_level->Draw( &m_scroll_x, &m_scroll_y, 900-TILE_W, 600-TILE_H, true );
    
    // Grid
    g_framework->DrawGrid( 30, 30, 900, 600 );
    
    // Tile selection box
    for( i=0; i<NUM_TILES; i++ )
        g_framework->Draw( m_level->GetTile( i ), i*TILE_W, m_tile_box.ypos );
    g_framework->Draw( m_selection_surf, m_tile_box.selected*TILE_W, m_tile_box.ypos );
    
    // Enemy selection box
    for( i=0; i<NUM_ENEMIES; i++ )
        g_framework->Draw( m_level->GetEnemySurf( i ), i*TILE_W, m_enemy_box.ypos );
    g_framework->Draw( m_selection_surf, m_enemy_box.selected*TILE_W, m_enemy_box.ypos );
    
    // Item selection box
    for( i=0; i<NUM_ITEMS; i++ )
        g_framework->Draw( m_level->GetItemSurf( i ), i*TILE_W, m_item_box.ypos );
    g_framework->Draw( m_selection_surf, m_item_box.selected*TILE_W, m_item_box.ypos );
    
    
    // Position
    g_framework->DrawText( "scroll_x/30=", EDIT_W - 100, EDIT_H - 100,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    g_framework->DrawNumber( m_scroll_x/30, EDIT_W - 20, EDIT_H - 100,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    g_framework->DrawText( "scroll_y/30=", EDIT_W - 100, EDIT_H - 80,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    g_framework->DrawNumber( m_scroll_y/30, EDIT_W - 20, EDIT_H - 80,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    // Help
    g_framework->DrawText( "Press 's' to save to 'levels/unnamed.ctux'!",
                            EDIT_W - 350, EDIT_H - 100,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    g_framework->DrawText( "Press 'p' to print the level into the stdout!",
                            EDIT_W - 350, EDIT_H - 80,
                            COLOR_YELLOW, F_SIZE_MEDIUM );
    
    // Mode
    g_framework->Draw( m_tile_mode, EDIT_W - 40, EDIT_H - 40 );
    g_framework->Draw( m_enemy_mode, EDIT_W - 80, EDIT_H - 40 );
    g_framework->Draw( m_item_mode, EDIT_W - 120, EDIT_H - 40 );
    
    switch( m_mode )
    {
        case MODE_TILE:
            g_framework->Draw( m_selection_surf, EDIT_W - 40, EDIT_H - 40 );
            break;
        case MODE_ENEMY:
            g_framework->Draw( m_selection_surf, EDIT_W - 80, EDIT_H - 40 );
            break;
        case MODE_ITEM:
            g_framework->Draw( m_selection_surf, EDIT_W - 120, EDIT_H - 40 );
            break;
    }
}
