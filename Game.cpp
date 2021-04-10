/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Game.cpp

#include "Game.h"

//
// CGame
// Constructor
//
CGame::CGame( SFramework_Setup *setup )
: CState()
{
    DEBUG( "Setting up the game state... ");
    
    m_setup = setup;
	m_worldmap_surfaces_loaded = false;
    
    // Don't exit
    SetAllDone( false );
    
    // Init load screen
    m_load_background = g_framework->LoadImage( load_filenames[LD_BACKGROUND] );
    m_load_circle = g_framework->LoadImage( load_filenames[LD_CIRCLE] );
    g_framework->SetColorKey( m_load_circle, COLOR_MAGENTA );
    CLoad *load = new CLoad( m_load_background, m_load_circle );
    
    load->AddAndDraw();
    
    // Init the menu
    m_menu = new CMenu( menu_filenames[MN_MENU], menu_filenames[MN_HEAD], false );
    m_menu->SetVisible( false );
    m_menu->ChangeSelection( SEL_NEW_GAME );
    
    load->AddAndDraw();
    
    // Init settings menu
    m_settings_menu = new CMenu( menu_filenames[MN_SETTINGS_MENU],
                                 menu_filenames[MN_HEAD], true );
    m_settings_menu->SetVisible( false );
    m_settings_menu->ChangeSelection( SEL_FULLSCREEN );
    
    // Init credits screen
    m_credits.visible = false;
    
    // Load credits images
    m_me = g_framework->LoadImage( credits_filenames[CREDITS_ME] );
    
    load->AddAndDraw();
    
    m_dogs = g_framework->LoadImage( credits_filenames[CREDITS_DOGS] );
    //g_framework->SetColorKey( m_dogs, COLOR_MAGENTA );
    
    // Init the worldmap
	load->AddAndDraw();
	m_worldmap = new CWorldmap();

	load->AddAndDraw();
	LoadWorldmapSurfaces( true );

	load->AddAndDraw();

    m_worldmap->SetActive( true );
    
    load->AddAndDraw();
    
    // Init the level
    m_level = NULL;

    load->AddAndDraw();
    
#ifndef NO_SOUND
	int i;
    // Load all songs
    for( i=0; i<NUM_SONGS; i++ )
        m_songs[i] = Mix_LoadMUS( song_filenames[ i ] );
    
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    Mix_PlayMusic( m_songs[SNG_STRANGE], -1 );
    load->AddAndDraw();
#endif

    // Delete load screen
    delete load;
    SDL_DestroyTexture( m_load_background );
    SDL_DestroyTexture( m_load_circle );

    DEBUG( "Game state is working!" );
}

//
// CGame
// Destructor
//
CGame::~CGame()
{    
    DEBUG( "Shutting down the game state... ");
    
#ifndef NO_SOUND
	int i;

    // Free songs from memory
    for( i=0; i<NUM_SONGS; i++ )
        Mix_FreeMusic( m_songs[i] );
#endif

    // Free credits images
    SDL_DestroyTexture( m_me );
    SDL_DestroyTexture( m_dogs );
    
    // Delete the worldmap
	UnloadWorldmapSurfaces();
	if( m_worldmap )
		delete m_worldmap;
    
    // Delete level
    if( m_level )
        delete m_level;
        
    // Delete menus
    delete m_menu;
    delete m_settings_menu;
    
    DEBUG( "Shutdown of game state complete! ");
}

//
// Input
// Game specific input
//
void CGame::Input()
{
    // Standard input processing
    CState::Input();
    
    // Normal handling
    if( !m_menu->GetVisible() && !m_settings_menu->GetVisible()
        && !m_worldmap->GetActive() )
    {
        if( m_keys[SDL_SCANCODE_ESCAPE] ) // Activate menu
        {
            if( m_released.key_escape )
            {
                m_released.key_escape = false;
                m_menu->SetVisible( true );
                return;
            }
        }
        else
            m_released.key_escape = true;
        
        // Walking      
        
        // Left  
        if( m_keys[SDL_SCANCODE_LEFT] )
        {
            if( !m_level->GetPlayer()->GetDying() )
                m_level->GetPlayer()->Walk( DIR_LEFT );                
        }
               
        // Right 
        if( m_keys[SDL_SCANCODE_RIGHT] )
        {
            if( !m_level->GetPlayer()->GetDying() )
                    m_level->GetPlayer()->Walk( DIR_RIGHT );                
        }
               
        // Standing
        if( !m_keys[SDL_SCANCODE_LEFT] && !m_keys[SDL_SCANCODE_RIGHT] )
            m_level->GetPlayer()->SetStanding( true );
                
        // Jumping        
        if( m_keys[SDL_SCANCODE_UP]&& !m_level->GetPlayer()->GetDying() )
        {
            if( m_released.key_return )
            {
                m_level->GetPlayer()->Jump();
                m_released.key_return = false;
            }
            else
            {
                m_level->GetPlayer()->IncreaseJump();
            }
        }
        else
            m_released.key_return = true;
        
        // Shooting
        if( m_keys[SDL_SCANCODE_SPACE] && !m_level->GetPlayer()->GetDying() )
        {
            if( m_released.key_space )
            {
                m_level->GetPlayer()->Shoot();
                m_released.key_space = false;
            }
        }
        else
            m_released.key_space = true;
    }
    // Menu visible
    else if( m_menu->GetVisible() && !m_settings_menu->GetVisible() )
    {
        if( m_keys[SDL_SCANCODE_ESCAPE] ) // Deactivate menu
        {
            if( m_released.key_escape )
            {
                m_released.key_escape = false;
                m_menu->SetVisible( false );
                return;
            }
        }
        else
            m_released.key_escape = true;
        
        if( m_keys[SDL_SCANCODE_UP] ) // Move selection head up
        {
            if( m_released.key_up )
            {
                if( m_menu->GetSelection() > 0 )
                    m_menu->ChangeSelection( m_menu->GetSelection() - 1 );
                else
                    m_menu->ChangeSelection( SEL_EXIT );
                m_released.key_up = false;
            }
        }
        else
            m_released.key_up = true;
        
        if( m_keys[SDL_SCANCODE_DOWN] ) // Move selection head down
        {
            if( m_released.key_down )
            {
                if( m_menu->GetSelection() < SEL_EXIT )
                    m_menu->ChangeSelection( m_menu->GetSelection() + 1 );
                else
                    m_menu->ChangeSelection( 0 );
                m_released.key_down = false;
            }
        }
        else
            m_released.key_down = true;
        
        if( m_keys[SDL_SCANCODE_RETURN] ) // Select
        {
            if( m_released.key_return )
            {
                switch( m_menu->GetSelection() )
                {
                    case SEL_EXIT:
                        m_done = true;
                        SetAllDone( true );
                        break;
                    case SEL_CREDITS:
                        m_credits.visible = true;
                        m_credits.remaining = CREDITS_TIME;
                        m_menu->SetVisible( false );
                        break;
                    case SEL_EDITOR:
                        m_setup->editor = true;
                        m_done = true;
                        break;
                    case SEL_DUEL:
                        m_setup->duel = true;
                        m_done = true;
                        break;
                    case SEL_NEW_GAME:
                        if( m_worldmap )
                            delete m_worldmap;
                        m_worldmap = new CWorldmap();
                        m_worldmap->SetActive( true );
                        m_menu->SetVisible( false );
#ifndef NO_SOUND
						Mix_HaltMusic();
						Mix_PlayMusic( m_songs[SNG_STRANGE], -1 );
#endif
                        break;
                    case SEL_SETTINGS:
                        m_settings_menu->SetVisible( true );
                        break;
                }
                m_released.key_return = false;
            }
        }
        else
            m_released.key_return = true;
    }
    // Settings menu visible
    else if( m_settings_menu->GetVisible() )
    {
        if( m_keys[SDL_SCANCODE_ESCAPE] ) // Deactivate menu
        {
            if( m_released.key_escape )
            {
                m_released.key_escape = false;
                m_settings_menu->SetVisible( false );
                return;
            }
        }
        else
            m_released.key_escape = true;
        
        if( m_keys[SDL_SCANCODE_UP] ) // Move selection head up
        {
            if( m_released.key_up )
            {
                if( m_settings_menu->GetSelection() > 0 )
                    m_settings_menu->ChangeSelection( m_settings_menu->GetSelection() - 1 );
                else
                    m_settings_menu->ChangeSelection( SEL_WINDOWED );
                m_released.key_up = false;
            }
        }
        else
            m_released.key_up = true;
        
        if( m_keys[SDL_SCANCODE_DOWN] ) // Move selection head down
        {
            if( m_released.key_down )
            {
                if( m_settings_menu->GetSelection() < SEL_WINDOWED )
                    m_settings_menu->ChangeSelection( m_settings_menu->GetSelection() + 1 );
                else
                    m_settings_menu->ChangeSelection( SEL_FULLSCREEN );
                m_released.key_down = false;
            }
        }
        else
            m_released.key_down = true;
        
        if( m_keys[SDL_SCANCODE_RETURN] ) // Select
        {
            if( m_released.key_return )
            {
                switch( m_settings_menu->GetSelection() )
                {
                    case SEL_FULLSCREEN:
                        g_framework->Fullscreen();
                        m_settings_menu->SetVisible( false );
                        m_menu->SetVisible( false );
                        break;
                    case SEL_WINDOWED:
                        g_framework->Windowed();
                        m_settings_menu->SetVisible( false );
                        m_menu->SetVisible( false );
                        break;
                }
                m_released.key_return = false;
            }
        }
        else
            m_released.key_return = true;        
    }
    // Worldmap
    else if( !m_settings_menu->GetVisible() && !m_menu->GetVisible() &&
            m_worldmap->GetActive() )
    {
        if( m_keys[SDL_SCANCODE_ESCAPE] ) // Activate menu
        {
            if( m_released.key_escape )
            {
                m_released.key_escape = false;
                m_menu->SetVisible( true );
                return;
            }
        }
        else
            m_released.key_escape = true;
            
        m_worldmap->Input( m_keys );
    }
}

//
// Update
// Game specific update
//
void CGame::Update()
{
    if( !m_menu->GetVisible()
        && !m_settings_menu->GetVisible()
        && !m_credits.visible ) // Normal update
    {
        if( !m_worldmap->GetActive() )
        {
            m_level->Update( &m_scroll_x, &m_scroll_y ); // Update the level
            
            // Get back to the worldmap he dies or exits the level
            if( g_framework->Coll( m_level->GetPlayer( 0 )->GetRect(), m_level->GetEndRect() )
                || m_level->GetPlayer( 0 )->GetRespawn() )
            {                
				LoadWorldmapSurfaces();

                m_worldmap->SetActive( true );
                m_worldmap->SetGameRunning( false );
                
                m_worldmap->SetLives( m_level->GetPlayer( 0 )->GetLives() );
                m_worldmap->SetScore( m_level->GetPlayer( 0 )->GetScore() );
                m_worldmap->SetBullets( m_level->GetPlayer( 0 )->GetNumBullets() );
                
                // Restart worldmap music
#ifndef NO_SOUND
                Mix_HaltMusic();
                Mix_PlayMusic( m_songs[SNG_STRANGE], -1 );
#endif
                
                // Special stuff for when the player has exit the level
                if( !m_level->GetPlayer( 0 )->GetRespawn() )
                {
                    m_worldmap->SetLevelDone( m_cur_level );                
#ifndef NO_SOUND
                    m_level->GetPlayer( 0 )->PlaySound( PSND_LEVEL_DONE );
#endif
                }

				// Delete level
				delete m_level;
				m_level = NULL;
            }
        }
        else // Worldmap
        {
            m_worldmap->Update();
            
            // Player wants to start a game
            if( m_worldmap->GetGameRunning() )
            {
#ifndef NO_SOUND
                // Stop worldmap music
                Mix_HaltMusic();
#endif
                
                m_worldmap->SetActive( false );
                m_worldmap->SetGameRunning( false );
                m_cur_level = m_worldmap->GetActLevel();

				UnloadWorldmapSurfaces();

                ChangeLevel( level_filenames[m_worldmap->GetActLevel()] );               
            }
        }
    }
    
    // Credits
    if( m_credits.visible && m_credits.remaining > 0 )
        m_credits.remaining--;
    else if( m_credits.visible )
        m_credits.visible = false;

}

//
// Draw
// Game specific draw
//
void CGame::Draw()
{    
    // Draw the level
    if( !m_credits.visible && !m_worldmap->GetActive() )
        m_level->Draw( &m_scroll_x, &m_scroll_y, SCR_W, SCR_H, false );
    // Fill the screen with blue for the credits screen
    else if( m_credits.visible )
        g_framework->FillRect( g_framework->GetScreen(), NULL, COLOR_RED );
    // Draw the worldmap
    else if( m_worldmap->GetActive() )
        m_worldmap->Draw();
    
    // Menu
    m_menu->Draw(); 
    m_settings_menu->Draw();
    
    // Credits
    if( m_credits.visible )
    {
        int w, h;
        // Me
        SDL_QueryTexture(m_me, nullptr, nullptr, &w, &h);
        g_framework->Draw( m_me, SCR_W-w, 0 );
        
        // Dogs
        SDL_QueryTexture(m_dogs, nullptr, nullptr, &w, &h);
        g_framework->Draw( m_dogs, SCR_W-w, SCR_H-h );
        
        // Title
        g_framework->DrawShadowText( "CommanderTux", 10, 10, COLOR_BLACK, COLOR_YELLOW, F_SIZE_BIG );
        g_framework->DrawShadowText( "Penguin In Space", 10, 40, COLOR_BLACK, COLOR_YELLOW, F_SIZE_MEDIUM );
        
        // Author
        g_framework->DrawShadowText( "2005 by André Schnabel, but don't care about any copyright:"
                                     "It's under the GPL dude!", 10, SCR_H-30,
                                COLOR_BLACK, COLOR_YELLOW, F_SIZE_MEDIUM );
        
        // Greets
        g_framework->DrawShadowText( "Greets to:", 10, 80, COLOR_BLACK, COLOR_WHITE, F_SIZE_BIG );
    
        g_framework->DrawShadowText( "My entire family",
                                     10, 120, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );        
        g_framework->DrawShadowText( "Matthias Semler a.k.a. James Bond 007, the grandmaster of gaming",
                                     10, 140, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
        g_framework->DrawShadowText( "Thomas Müller, some unknown beta tester",
                                     10, 160, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
        g_framework->DrawShadowText( "Siegbert Reese, former computer science teacher of mine",
                                     10, 180, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
                                     
        g_framework->DrawShadowText( "Our vivacious dogs (those on the picture)",
                                     10, 220, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
        g_framework->DrawShadowText( "If I hadn't spent so much time walking with them,",
                                     10, 240, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
        g_framework->DrawShadowText( "CommanderTux would had many more features ;-)",
                                     10, 260, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
                                     
        g_framework->DrawShadowText( "Everybody I itentionally left out...",
                                     10, 300, COLOR_BLACK, COLOR_WHITE, F_SIZE_MEDIUM );
    }
}

/*

OBSOLETE: There is no need for such a function when we have a worldmap!

//
// NextLevel
//
void CGame::NextLevel()
{
    // Iterate current level and go into it
    m_cur_level++;
    ChangeLevel( level_filenames[ m_cur_level ] );
}
*/

//
// ChangeLevel
//
void CGame::ChangeLevel( const char *filename )
{
    // Delete old level if available
    if( m_level )
        delete m_level;
    
    // Load new one
    
    switch( m_cur_level )
    {
        case 0: // Level 1: Ice
            m_level = new CLevel( filename, false, false, background_filenames[BACKGROUND_2] );
            break;
        case 1: // Level 2: Forest
            m_level = new CLevel( filename, false, false, background_filenames[BACKGROUND_4] );
            break;
        case 2: // Level 3: Clouds
            m_level = new CLevel( filename, false, false, background_filenames[BACKGROUND_3] );
            break;
        case 3: // Level 4: Stones -> default
            m_level = new CLevel( filename, false, false );
            break;
        default: // No background specified
            m_level = new CLevel( filename, false, false );
    }    
    
#ifndef NO_SOUND
    // Start ingame music
    Mix_PlayMusic( m_songs[SNG_TECHI], -1 );
#endif
    
    // Set scroll position based on the player position set by the level
    m_scroll_x = m_level->GetPlayer( 0 )->GetRect()->x;
    m_scroll_y = m_level->GetPlayer( 0 )->GetRect()->y;
    
    // Center if possible
    if( m_scroll_x > SCR_W / 2 )
        m_scroll_x-=SCR_W / 2;
    else
        m_scroll_x = 0;
    
    // Prevent from scrolling to anything out of border
	if( m_scroll_y > SCR_H / 2 )
		m_scroll_y-=SCR_H / 2;
	else
		m_scroll_y = 0;
        
    // Set right score and lives
    m_level->GetPlayer( 0 )->SetLives( m_worldmap->GetLives() );
    m_level->GetPlayer( 0 )->SetScore( m_worldmap->GetScore() );
    m_level->GetPlayer( 0 )->SetNumBullets( m_worldmap->GetBullets() );
}

//
// LoadWorldmapSurfaces
//
void CGame::LoadWorldmapSurfaces( bool first_time )
{
    CLoad *load;
	int i, j;

	if( !m_worldmap_surfaces_loaded )
	{
        if( !first_time )
        {
    		// Init load screen
    		m_load_background = g_framework->LoadImage( load_filenames[LD_BACKGROUND] );
    		m_load_circle = g_framework->LoadImage( load_filenames[LD_CIRCLE] );
    		g_framework->SetColorKey( m_load_circle, COLOR_MAGENTA );
    		load = new CLoad( m_load_background, m_load_circle );
    		
    		load->AddAndDraw();
        }	

		// Init worldmap images
		m_world_surface = g_framework->LoadImage( worldmap_filenames[WORLD] );

        if( !first_time )
    		load->AddAndDraw();
    
		// Worldmap player images
		for( i=0; i<NUM_W_P_IMAGES; i++ )
		{
	        // Walking and standing
			for( j=0; j<2; j++ )
			{
	            m_world_player_surfaces[j][i] = g_framework->LoadImage( w_player_filenames[j][i] );
				g_framework->SetColorKey( m_world_player_surfaces[j][i], COLOR_MAGENTA );
			}

            if( !first_time )
    			load->AddAndDraw();
		}
	
		// Reset images of the worldmap object
		m_worldmap->SetImages( m_world_surface, m_world_player_surfaces );

        if( !first_time )
    		load->AddAndDraw();

		m_worldmap_surfaces_loaded = true;

        if( !first_time )
        {
    		// Free load screen
    		delete load;
    		SDL_DestroyTexture( m_load_background );
    		SDL_DestroyTexture( m_load_circle );
        }
	}
}

//
// UnloadWorldmapSurfaces
//
void CGame::UnloadWorldmapSurfaces()
{
	int i, j;

	if( m_worldmap_surfaces_loaded )
	{
		// Free worldmap surfaces
		SDL_DestroyTexture( m_world_surface );
    
		for( i=0; i<NUM_W_P_IMAGES; i++ )
		{
	        for( j=0; j<2; j++ )
				SDL_DestroyTexture( m_world_player_surfaces[j][i] );
		}

		m_worldmap_surfaces_loaded = false;
	}
}

//
// CMenu
// Constructor
//
CMenu::CMenu(	const char *img_name,
				const char *head_name,
				bool settings_menu )
{
    // Copy function parameter to member var
    m_settings_menu = settings_menu;
    
    // Load menu image and set color key
    m_image = g_framework->LoadImage( img_name );
    g_framework->SetColorKey( m_image, COLOR_MAGENTA );
    
    // Load head image and set color key
    m_head_surf = g_framework->LoadImage( head_name );
    g_framework->SetColorKey( m_head_surf, COLOR_MAGENTA );
    
    // Init animated sprite object for the head
    m_head_spr = new CAnim_Sprite( m_head_surf, 15, 15, 50 );
    m_head_move_eye = false;
    
#ifndef NO_SOUND
    // Load selection sound
    m_sel_sound = Mix_LoadWAV( menu_filenames[MN_SEL_SND] );
#endif
}

//
// CMenu
// Destructor 
//
CMenu::~CMenu()
{
    // Delete animated head sprite
    delete m_head_spr;
#ifndef NO_SOUND
    // Free memory from selection sound
    Mix_FreeChunk( m_sel_sound );
#endif
    // Delete menu image
    SDL_DestroyTexture( m_image );
    // Free head image
    SDL_DestroyTexture( m_head_surf );
}

//
// Draw
// Menu and head for selection.
//
void CMenu::Draw()
{
    if( m_visible )
    {
        int w,h;
        SDL_QueryTexture(m_image, nullptr, nullptr, &w, &h);

        if( !m_settings_menu )
        {
            // Draw head sprite
            m_head_spr->SetPos((SCR_W-w)/2 + 20,
                            (SCR_H-h) / 2 + 35 + 30*m_selection);
            // Draw menu image
            g_framework->Draw( m_image, (SCR_W-w) / 2, (SCR_H-h) / 2 );
    
        }
        else
        {
            // Draw head sprite
            m_head_spr->SetPos(SCR_W-w-20 + 35,
                            (SCR_H-h) / 2 + 50 + 85*m_selection);
            
            // Draw menu image
            g_framework->Draw( m_image, SCR_W-w-20, (SCR_H-h) / 2 );
        }
        
        if( !(rand() % 50) && !m_head_move_eye )
        {
            m_head_move_eye = true;
        }
        
        if( m_head_move_eye )
        {
            m_head_spr->Draw( true );
            
            if( m_head_spr->GetActImage() == 0 )
                m_head_move_eye = false;       
        }
        
        if( !m_head_move_eye )
            m_head_spr->Draw( false );
    }
}

//
// ChangeSelection
//
void CMenu::ChangeSelection( int selection )
{
#ifndef NO_SOUND
    // Play selection sound
    Mix_PlayChannel( -1, m_sel_sound, 0 );
#endif
    // Set selection to the new value
    m_selection = selection;
}

//
// GetSelection
//
int CMenu::GetSelection()
{
    return m_selection;
}
