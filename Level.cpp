/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Level.cpp

#include "Level.h"

//
// CLevel
// Constructor
//
CLevel::CLevel( const char *filename, bool editor, bool duel,
                const char *background_filename )
{    
    int i, j;    
    
    DEBUG( "Initializing the level... " );
    
    // Init vars
    m_level = NULL;
    m_duel = duel;
    m_editor = editor;
    m_gameover = false;
    
	// Init load screen
	m_load_background = g_framework->LoadImage( load_filenames[LD_BACKGROUND] );
    m_load_circle = g_framework->LoadImage( load_filenames[LD_CIRCLE] );
    g_framework->SetColorKey( m_load_circle, COLOR_MAGENTA );
    CLoad *load = new CLoad( m_load_background, m_load_circle );

	// Init border
    m_border.x = 0; 
    m_border.y = LVL_H_MAX * TILE_H;
    
    // Just make sure we don't do anything wrong!
    m_enemies = NULL;
    
    // Allocate the level
    m_level = new SLevel();    
    
    // Load m_level struct from file if filename is given
    if( filename )
        Load( filename );

	load->AddAndDraw();
    
    // Load tile images
    for( i=0; i<NUM_TILES; i++ )
    {
        m_tiles[i] = g_framework->LoadImage( tile_filenames[i] );
        g_framework->SetColorKey( m_tiles[i], COLOR_MAGENTA );
    }        
    
	load->AddAndDraw();

    // Init enemies + items   
    if( !m_duel ) // There are no enemies in the duel mode
    {    
        // Load enemy images
        for( i=0; i<NUM_ENEMIES; i++ )
        {
            for( j=0; j<NUM_ACTOR_IMAGES; j++ )
            {
                m_enemy_surfaces[i][j]
                    = g_framework->LoadImage( enemy_filenames[i][j] );
                g_framework->SetColorKey( m_enemy_surfaces[i][j], COLOR_MAGENTA );
            }
        }

		load->AddAndDraw();
        
        // Load item images
        for( i=0; i<NUM_ITEMS; i++ )
        {
            m_item_surfaces[i] = g_framework->LoadImage( item_filenames[i] );
            g_framework->SetColorKey( m_item_surfaces[i], COLOR_MAGENTA );
        }
        
		load->AddAndDraw();

        // Stuff for the game mode
        if( !m_editor )
        {
            // Create the linked list of enemies and items
            m_enemies = new CLinkedList< CEnemy >;
            m_items = new CLinkedList< CItem >;
            
            // Convert enemies from tile numbers in the content array of the SLevel struct
            // to real objects of the CEnemy type, not interesting in the editor mode
            for( i=0; i<LVL_H_MAX; i++ )
            {
                for( j=0; j<LVL_W_MAX; j++ )
                {
                    if( m_level->content[i][j] >= ENEMY_SHIFT &&
                        m_level->content[i][j] < ITEM_SHIFT )
                    {
                        CEnemy *new_enemy =
                            AddEnemy( m_level->content[i][j] - ENEMY_SHIFT, j*TILE_W, i*TILE_H );
                        m_level->content[i][j] = TILE_EMPTY;
                        m_enemies->Add( new_enemy );
                    }
                    else if( m_level->content[i][j] >= ITEM_SHIFT )
                    {
                        CItem *new_item =
                            AddItem( m_level->content[i][j] - ITEM_SHIFT, j*TILE_W, i*TILE_H );
                        m_level->content[i][j] = TILE_EMPTY;
                        m_items->Add( new_item );
                    }
                }
            }

			load->AddAndDraw();

        } // !m_editor
    
    } // !m_duel
    
	load->AddAndDraw();

    // Stuff that is only for the game and duel mode
    if( !m_editor )
    {
        // For player start positions
        int tmp_s_x, tmp_s_y;
        
        // This rectangle is made to help us realizing a collision between the player
        // and the end tile.
        m_end_rect.w = TILE_W + END_OVERLAY;
        m_end_rect.h = TILE_H + END_OVERLAY;
        
        // Make the end tile usable for the game
        for( i=0; i<LVL_H_MAX; i++ )
        {
            for( j=0; j<LVL_W_MAX; j++ )
            {
                if( m_level->content[i][j] == TILE_END )
                {
                    m_level->content[i][j] = TILE_END_VISIBLE;
                    m_end_rect.x = TILE_W * j - END_OVERLAY;
                    m_end_rect.y = TILE_H * i - END_OVERLAY;
                    break;
                }
            }
        }
        
        // Load background image
        if( !m_editor )
            m_background = 
                g_framework->LoadImage( background_filename );
        
		load->AddAndDraw();

        // Init players    
        // Load player images        
        for( i=0; i<MAX_PLAYERS; i++ )
        {
            for( j=0; j<NUM_ACTOR_IMAGES; j++ )
            {
                m_player_surfaces[i][j] = g_framework->LoadImage( player_filenames[i][j] );
                g_framework->SetColorKey( m_player_surfaces[i][j], COLOR_MAGENTA );
            }
        }
        
        // Load bullet surface
        m_bullet_surf = g_framework->LoadImage( bullet_filenames[BULLET] );
        g_framework->SetColorKey( m_bullet_surf, COLOR_MAGENTA );
        
        // Load gun surface
        
        // Left
        m_gun_surf[0] = g_framework->LoadImage( gun_filenames[GUN_LEFT] );
        g_framework->SetColorKey( m_gun_surf[0], COLOR_MAGENTA );
        
        // Right
        m_gun_surf[1] = g_framework->LoadImage( gun_filenames[GUN_RIGHT] );
        g_framework->SetColorKey( m_gun_surf[1], COLOR_MAGENTA );
        
#ifndef NO_SOUND
        // Load player sounds
        for( i=0; i<NUM_PLAYER_SOUNDS; i++ )
            m_player_snds[i] = Mix_LoadWAV( player_sound_filenames[i] );
#endif
        
        // So we don't scroll out of the border
        if( !m_duel )            
            GetBorderPoint( &m_border.x, &m_border.y );
        
		load->AddAndDraw();

        // Start position...
        // ...for the first player...
        GetStartPos( &tmp_s_x, &tmp_s_y );
#ifndef NO_SOUND
        m_players[0] =
        new CPlayer( tmp_s_x, tmp_s_y, m_level, m_player_surfaces[0],
        m_player_snds, 0, m_duel, m_border, m_enemies, m_bullet_surf, m_gun_surf );
#else
        m_players[0] =
        new CPlayer( tmp_s_x, tmp_s_y,
                    m_level, m_player_surfaces[0], 0, m_duel,
                        m_border, m_enemies, m_bullet_surf, m_gun_surf );
#endif  
		load->AddAndDraw();

        // ...and for the others
        if( m_duel )
        {
            for( i=1; i<MAX_PLAYERS; i++ )
            {
                GetStartPos( &tmp_s_x, &tmp_s_y );
#ifndef NO_SOUND
                m_players[i] =
                    new CPlayer( tmp_s_x, tmp_s_y,
                    m_level, m_player_surfaces[i],
                    m_player_snds, i, true, m_border, 0, m_bullet_surf, m_gun_surf );
#else
                m_players[i] =
                    new CPlayer( tmp_s_x, tmp_s_y, m_level,
                                 m_player_surfaces[i], i, true, m_border,
                                 0, m_bullet_surf, m_gun_surf );
#endif
            }
        }        
        
    } // !m_editor

	// Delete load screen
    delete load;
    SDL_DestroyTexture( m_load_background );
    SDL_DestroyTexture( m_load_circle );
    
    DEBUG( "Level initialized!" );
}

//
// CLevel
// Destructor
//
CLevel::~CLevel()
{
    int i, j;
    
    DEBUG( "Closing the level... ");	
	
	if( !m_editor )
    {
        // Delete the first player
        delete m_players[0];
	
    	// Delete others for duel mode
    	if( m_duel )
      	{
            for( i=1; i<MAX_PLAYERS; i++ )
            {
                delete m_players[i];
            }
        }	
        
    	// Free the background surface
    	SDL_DestroyTexture( m_background );
    }
	
	
	// Free tile surfaces
	for( i=0; i<NUM_TILES; i++ )
		SDL_DestroyTexture( m_tiles[i] );
	
	if( !m_editor )
	{
    	// Free player surfaces
    	for( i=0; i<MAX_PLAYERS; i++ )
    	{
        	for( j=0; j<NUM_ACTOR_IMAGES; j++ )
        	   SDL_DestroyTexture( m_player_surfaces[i][j] );
        }
        
        // Free bullet surface
        SDL_DestroyTexture( m_bullet_surf );
        
        // Free gun surface
        SDL_DestroyTexture( m_gun_surf[0] );
        SDL_DestroyTexture( m_gun_surf[1] );

#ifndef NO_SOUND        
        // Free player sounds
        for( i=0; i<NUM_PLAYER_SOUNDS; i++ )
            Mix_FreeChunk( m_player_snds[i] );
#endif
    }
	
	// Enemies are only interesting when not in duel mode
	if( !m_duel )
	{
        // Free enemy surfaces	
    	for( i=0; i<NUM_ENEMIES; i++ )
    	{
            for( j=0; j<NUM_ACTOR_IMAGES; j++ )
                SDL_DestroyTexture( m_enemy_surfaces[i][j] );
        }
        
        // Free item surfaces
        for( i=0; i<NUM_ITEMS; i++ )
            SDL_DestroyTexture( m_item_surfaces[i] );
        
        // Delete list of enemy and item objects
        if( !m_editor )
        {
            delete m_enemies;
            delete m_items;
        }
	   
    }
    
    // Delete the entire level structure
    if( m_level )
        delete m_level;
    
    DEBUG( "Level closed!" );
}

//
// Load
// Make sure that the SLevel struct you want to load from a binary file has the
// same structure as the SLevel struct in the SLevel header file.
// TODO: Make it more C++-ish
//
void CLevel::Load( const char *filename )
{
    FILE *fp;
    
    DEBUG( "Loading level..." );    
    fp = fopen( filename, "r" );
    fread( m_level, sizeof( SLevel ), 1, fp );
    fclose( fp );
    printf( "CTux_Msg: Level name '%s'\n", filename );
    DEBUG( "Level loaded!" );
}

//
// Save
// Mostly for the editor
// TODO: Make it more C++-ish
//
void CLevel::Save( const char *filename )
{
    FILE *fp;
    
    DEBUG( "Saving level..." );    
    fp = fopen( filename, "w" );
    fwrite( m_level, sizeof( SLevel ), 1, fp );
    fclose( fp );
    DEBUG( "Level saved..." );
}

//
// GetStartPos
// Returns a start position by looking for a start tile in the map and makes
// it empty. For duel mode you need to put four start tiles in the map, so 
// every player has a start position. For the normal game mode use only
// one start tile!
//
void CLevel::GetStartPos( int *x, int *y )
{
    int i, j;
    
    for( i=0; i<LVL_H_MAX; i++ )
    {
        for( j=0; j<LVL_W_MAX; j++ )
        {
            if( m_level->content[i][j] == TILE_START )
            {
                *x = j*TILE_W;
                *y = i*TILE_H;
                m_level->content[i][j] = TILE_EMPTY;
                return;
            }
        }
    }
}

//
// Update
//
void CLevel::Update( int *scroll_x, int *scroll_y )
{
    int i, j;
    
    if( !m_gameover ) {
    
    // Update first player, this is enough for singleplayer
    m_players[0]->Update( scroll_x, scroll_y );
    
    // In duel mode we also need to care about the other players, collisions and stuff
    if( m_duel )
    {
        
        for( i=1; i<MAX_PLAYERS; i++ )
        {
            // Update the player
            m_players[i]->Update( scroll_x, scroll_y );
            
            // Coll with other player
            for( j=0; j<MAX_PLAYERS; j++ )
            {
                // Different players collide
                if( g_framework->Coll( m_players[i]->GetRect(),
                                       m_players[j]->GetRect() )
                    && i!=j )
                {   
                    // Obviously player[i] is the winner                 
                    if( m_players[i]->GetFalling() && !m_players[j]->GetFalling() )
                        m_players[i]->Frag( m_players[j] );                    
                    // Obviously player[j] is the winner
                    else if( m_players[j]->GetFalling() && !m_players[i]->GetFalling() )
                        m_players[j]->Frag( m_players[i] );                    
                    // Otherwise higher player is the winner
                    else if( m_players[i]->GetFalling() && m_players[j]->GetFalling() )
                    {
                        // player[i] is higher ( y is smaller )
                        if( m_players[i]->GetRect()->y < m_players[j]->GetRect()->y )
                            m_players[i]->Frag( m_players[j] );
                        // player[j] is higher
                        else
                            m_players[j]->Frag( m_players[i] );
                    }
                }
            }
        }
    }
    else // normal mode
    {
        // Gameover?
        if( m_players[0]->GetDead() )
            m_gameover = true;
        
        // Enemies
        for( i=0; i<m_enemies->GetNumberOfElements(); i++ )
        {
            // Update the enemy before
            m_enemies->GetData( i )->Update( scroll_x, scroll_y );
            
            // Coll between player and enemy
            // Players falls onto enemy and kills it            
            
            if( g_framework->Coll( m_enemies->GetData( i )->GetRect(), m_players[0]->GetRect() )
                && m_players[0]->GetFalling() )
            {
                if( !m_enemies->GetData( i )->GetDying() )
                {
                    if( m_enemies->GetData( i )->GetId() != ENEMY_STONEY
                        && m_enemies->GetData( i )->GetId() != ENEMY_PLANT )
                    {
                        m_enemies->GetData( i )->Die();
                        m_players[0]->IncrementScore( m_enemies->GetData( i )->GetScoreValue(),
                                  m_enemies->GetData( i )->GetRect()->x
                                  +m_enemies->GetData( i )->GetRect()->w/2 -5,
                                  m_enemies->GetData( i )->GetRect()->y, ENEMY_KILLED );
                    }
                    else
                        m_players[0]->Die(); // Stoney and plant kill the player everytime
                    
                }
            }
            // Otherwise when the player is not falling he is killed by the enemy
            else if( g_framework->Coll( m_enemies->GetData( i )->GetRect(), m_players[0]->GetRect() ) )
            {
                if( !m_enemies->GetData( i )->GetDying() )
                    m_players[0]->Die();
            }
        }
        
        // Items
        for( i=0; i<m_items->GetNumberOfElements(); i++ )
        {
            // Coll between player and item
            // Everytime the player scores or gets other stuff
            if( g_framework->Coll( m_items->GetData( i )->GetRect(), m_players[0]->GetRect() ) )
            {
                if( m_items->GetData( i )->GetId() != ITEM_BULLETS
                    && m_items->GetData( i )->GetId() != ITEM_LIVE ) // Normal item
                {
                    m_players[0]->IncrementScore( m_items->GetData( i )->GetScore(),
                                        m_items->GetData( i )->GetRect()->x +
                                        m_items->GetData( i )->GetRect()->w / 2,
                                        m_items->GetData( i )->GetRect()->y, ITEM_COLLECTED );
                }
                else if( m_items->GetData( i )->GetId() == ITEM_BULLETS ) // Bullets
                    m_players[0]->AddBullets( 10 );
                else // Live
                    m_players[0]->AddLive();
                
                m_items->Delete( i );
            }
        }
    }
    
    } // !m_gameover
}

//
// Draw
//
void CLevel::Draw( int *scroll_x, int *scroll_y, int w, int h, bool hide_players )
{
    int i, j;
    
    m_hide_players = hide_players;

    // Draw background with pseudo scrolling
    // We have to draw the background four times... :-(
    // Please make sure you've set a clipping rect for the screen before!!!
    if( !m_editor )
    {
        int m_background_w = g_framework->GetTextureWidth(m_background);
        int m_background_h = g_framework->GetTextureHeight(m_background);

        // Left Up
        g_framework->Draw(  m_background,
                            -*scroll_x%m_background_w,
                            -*scroll_y%m_background_h  );
        
        // Right Up
        g_framework->Draw(  m_background,
                            m_background_w-*scroll_x%m_background_w,
                            -*scroll_y%m_background_h );
        
        // Left Down
        g_framework->Draw(  m_background,
                            -*scroll_x%m_background_w,
                            m_background_h-*scroll_y%m_background_h  );
        
        // Right Down
        g_framework->Draw(  m_background,
                            m_background_w-*scroll_x%m_background_w,
                            m_background_h-*scroll_y%m_background_h );
    }
    
    
    // Draw tiles and enemies
    
    // Prevent drawing out of border
    if( (h+*scroll_y) / TILE_H > LVL_H_MAX - 1 || *scroll_y < 0 )
        return;
    if( (w+*scroll_x) / TILE_W > LVL_W_MAX - 1 || *scroll_x < 0 )
        return;
    
    // Drawing every tile visible in the specific field
    for( i=*scroll_y/TILE_H; i<=(h+*scroll_y)/TILE_H; i++ ) // vertical
    {
        for( j=*scroll_x/TILE_W; j<=(w+*scroll_x)/TILE_W; j++ ) // horizontal
        {
            if( m_level->content[i][j] != TILE_EMPTY ) // only draw not empty tiles...
            {
                // only draw start or end tiles in editor mode
                if( m_level->content[i][j] != TILE_START &&
                    m_level->content[i][j] != TILE_END ||
                    m_editor )
                {
                    if( m_level->content[i][j] < ENEMY_SHIFT ) // normal tile
                    {
                        g_framework->Draw( m_tiles[m_level->content[i][j]],
                                j*TILE_W-*scroll_x, i*TILE_H-*scroll_y );
                    }
                    else if( m_editor
                            && m_level->content[i][j] < ITEM_SHIFT ) // enemy // editor vers.
                    {
                        g_framework->Draw( m_enemy_surfaces[m_level->content[i][j]-ENEMY_SHIFT][0],
                        j*TILE_W-*scroll_x, i*TILE_H-*scroll_y );
                    }
                    else if( m_editor ) // item // editor vers.
                    {
                        g_framework->Draw( m_item_surfaces[m_level->content[i][j]-ITEM_SHIFT],
                        j*TILE_W-*scroll_x, i*TILE_H-*scroll_y );
                    }
                }
            }
        }
    }
    
    // Draw enemies and items
    if( !m_editor && !m_duel )
    {
        // Enemies
        for( i=0; i<m_enemies->GetNumberOfElements(); i++ )
            m_enemies->GetData( i )->Draw( scroll_x, scroll_y );
        
        // Items
        for( i=0; i<m_items->GetNumberOfElements(); i++ )
            m_items->GetData( i )->Draw( scroll_x, scroll_y );        
    }
    
    // Draw players
    if( !m_editor && !hide_players )
    {
        // Player 1 for singleplayer mode
        m_players[0]->Draw( scroll_x, scroll_y, 10 );
        
        // The others for duel mode
        if( m_duel )
        {
            for( i=1; i<MAX_PLAYERS; i++ )
                m_players[i]->Draw( scroll_x, scroll_y, 10*i+10 );
        }
    }
    
    // Gameover?
    if( m_gameover )
    {
        g_framework->DrawShadowText( "Gameover!", SCR_W/2-30, SCR_H/2-10, COLOR_BLACK,
                                        COLOR_YELLOW, F_SIZE_BIG );
    }
}

//
// AddEnemy
// Returns a pointer to a new enemy of a specific type
//
CEnemy *CLevel::AddEnemy( int enemy_nr, int x, int y )
{   
    CEnemy *cur_enemy;
    
    switch( enemy_nr )
    {
        case ENEMY_BLOB:
            cur_enemy =
                new CBlob( x, y, m_level, m_enemy_surfaces[ENEMY_BLOB], BLOB_WALK_SPEED );
            break;
        case ENEMY_ROBOT:
            cur_enemy =
                new CRobot( x, y, m_level, m_enemy_surfaces[ENEMY_ROBOT], ROBOT_WALK_SPEED );
            break;
        case ENEMY_STONEY:
            cur_enemy = new CStoney( x, y, m_level, m_enemy_surfaces[ENEMY_STONEY] );
            break;
        case ENEMY_PLANT:
            cur_enemy = new CPlant( x, y, m_level, m_enemy_surfaces[ENEMY_PLANT] );
            break;
		case ENEMY_SNAIL:
			cur_enemy = new CSnail( x, y, m_level, m_enemy_surfaces[ENEMY_SNAIL], SNAIL_WALK_SPEED );
			break;
		case ENEMY_ROCKETBALL:
			cur_enemy = new CRocketBall( x, y, m_level, m_enemy_surfaces[ENEMY_ROCKETBALL], ROCKETBALL_WALK_SPEED );
			break;
        default:
            return NULL;
    }
    
    return cur_enemy;
}

//
// AddItem
// Returns a pointer to a new item of a specific type
//
CItem *CLevel::AddItem( int item_nr, int x, int y )
{    
    CItem *cur_item;    
    cur_item = new CItem( m_item_surfaces[item_nr], item_nr );
    cur_item->SetPos( x, y );
    return cur_item;
}

//
// PrintLevelContent
// Mostly for debugging purposes
//
void CLevel::PrintLevelContent()
{
    int i, j;
    
    printf( "\nLevel Content:\n" );
    
    for( i=0; i<LVL_H_MAX; i++ ) // Horizontal
    {
        for( j=0; j<LVL_W_MAX; j++ ) // Vertical
        {
            // Format
            printf( "\t" );
            // Output
            printf( "%d", m_level->content[i][j] );
        }
        printf( "\n" );
    }
}

//
// GetBorderPoint
// Gets the position of the tile that is most right and up
// for scrolling in game mode
//
void CLevel::GetBorderPoint( int *x, int *y )
{
    int i, j;
        
    for( i=0; i<LVL_H_MAX; i++ ) // Horizontal
    {
        for( j=0; j<LVL_W_MAX; j++ ) // Vertical
        {
            if( m_level->content[i][j] != TILE_EMPTY )
            {
                if( (j+1)*TILE_W > *x )
                    *x = (j+1)*TILE_W;
                if( i*TILE_H < *y )
                    *y = i*TILE_H;
            }
        }
    }
}
