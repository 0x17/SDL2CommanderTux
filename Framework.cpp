/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Framework.cpp
// Our underlying framework. It handles initializing of the SDL as well as video
// stuff. Note: It doesn't contain the input handling stuff. For this you have
// to look at State.h.

#include "Framework.h"

CFramework *g_framework = NULL;

//
// ParseArgs
// Does what every command line processing function does.
//
SFramework_Setup ParseArgs( int argc, char *argv[], bool *stop )
{
    int i;
    SFramework_Setup setup;
    
	*stop = false;

    // Process every single argument
    for( i=0; i<argc; i++ )
    {
        // Show all possible arguments
        if( !strcmp( argv[i], "--help" ) )
        {
            printf( "CommanderTux Help\n"
            "=================\n" );
            printf( "Command line parameters:\n" );
            printf( "--nodoublebuf  - prevents CTux from using a second buffer\n"
            "--fullscreen    - makes CTux fullscreen\n"
            "--duel         - duel mode"
            "--editor   - the Leveleditor" );
            *stop = true;
        }
    
        // Fullscreen mode instead of the default windowed video mode
        if( !strcmp( argv[i], "--fullscreen" ) )
        {
            setup.scr_flags += SDL_WINDOW_BORDERLESS;
            setup.fullscreen = true;
        }
        
        // Duel mode
        if( !strcmp( argv[i], "--duel" ) )
            setup.duel = true;
        
        // Editor mode
        if( !strcmp( argv[i], "--editor" ) )
            setup.editor = true;
    }
    
    return setup;
}

//
// CFramework
// Constructor
//
CFramework::CFramework( SFramework_Setup *setup ) : m_window(nullptr), m_renderer(nullptr), m_screen(nullptr)
{
    int result;
    
    DEBUG( "Setting up the framework..." );
    
    // Init the setup struct
    m_setup = new SFramework_Setup;
    
    // Start the random number generator
    srand( (unsigned int) time( NULL ) );
    
    // Copy the given setup structure, if there is any
    if( setup )
        memcpy( m_setup, setup, sizeof( SFramework_Setup ) );
        
    // Init the SDL
    DEBUG( "Init the SDL..." );
    result = SDL_Init( SDL_INIT_VIDEO );
    if( result == -1 )
    {
        Error( ERR_SDL );
        return;
    }
    m_loaded.sdl = true;
    DEBUG( "SDL is started!" );
    
    // Init video
    DEBUG( "Setting up video..." );
    SetVideoMode();
    
    if( !m_screen )
    {
        Error( ERR_VID );
        return;
    }
    if( m_setup->fullscreen ) // so nothing starts before the video is visible!
        SDL_Delay( 2000 );
    
    //SDL_SetClipRect( m_screen, NULL );
    SetCaption();
    
    // Colors
    m_sdl_colors.magenta.r = 255; m_sdl_colors.magenta.g = 0; m_sdl_colors.magenta.b = 255;
    m_sdl_colors.black.r = 0; m_sdl_colors.black.g = 0; m_sdl_colors.black.b = 0;
    m_sdl_colors.white.r = 255; m_sdl_colors.white.g = 255; m_sdl_colors.white.b = 255;
    m_sdl_colors.red.r = 255; m_sdl_colors.red.g = 0; m_sdl_colors.red.b = 0;
    m_sdl_colors.green.r = 0; m_sdl_colors.green.g = 255; m_sdl_colors.green.b = 0;
    m_sdl_colors.blue.r = 0; m_sdl_colors.blue.g = 0; m_sdl_colors.blue.b = 255;
    m_sdl_colors.bright_blue.r = 150; m_sdl_colors.bright_blue.g = 150; m_sdl_colors.bright_blue.b = 255;
    m_sdl_colors.gray.r = 150; m_sdl_colors.gray.g = 150; m_sdl_colors.gray.b = 150;
    m_sdl_colors.yellow.r = 255; m_sdl_colors.yellow.g = 255; m_sdl_colors.yellow.b = 0;
    
    m_uint32_colors.magenta = SDL_MapRGB( m_screen->format, 255, 0, 255 );
    m_uint32_colors.black = SDL_MapRGB( m_screen->format, 0, 0, 0 );
    m_uint32_colors.white = SDL_MapRGB( m_screen->format, 255, 255, 255 );
    m_uint32_colors.red = SDL_MapRGB( m_screen->format, 255, 0, 0 );
    m_uint32_colors.green = SDL_MapRGB( m_screen->format, 0, 255, 0 );
    m_uint32_colors.blue = SDL_MapRGB( m_screen->format, 0, 0, 255 );
    m_uint32_colors.bright_blue = SDL_MapRGB( m_screen->format, 150, 150, 255 );
    m_uint32_colors.gray = SDL_MapRGB( m_screen->format, 150, 150, 150 );
    m_uint32_colors.yellow = SDL_MapRGB( m_screen->format, 255, 255, 0 );
    
    m_loaded.vid = true;
    DEBUG( "Video setup successfull!" );
    
    // Init ttf
    TTF_Init();
    
    // Load all fonts
    m_font_small = TTF_OpenFont( font_filenames[FNT_FONT], 10 );
    m_font_medium = TTF_OpenFont( font_filenames[FNT_FONT], 12 );
    m_font_big = TTF_OpenFont( font_filenames[FNT_FONT], 24 );
    m_font_biggest = TTF_OpenFont( font_filenames[FNT_FONT], 28 );
    
    m_loaded.ttf = true;
    
    // Init audio
#ifndef NO_SOUND
    result = Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048 );
    if( result == -1 )
    {
        Error( ERR_SND );
        return;
    }
    m_loaded.snd = true;
#endif

    m_loaded.everything = true;
    
    // Set this to the global framework
    g_framework = this;
    
    DEBUG( "Framework is started!" );
}

//
// CFramework
// Destructor
//
CFramework::~CFramework()
{
    DEBUG( "Shutdown framework..." );
    
    // Reactivate cursor
    ShowCursor( true );
    
#ifndef NO_SOUND
    if( m_loaded.snd )
    {
        // Close audio
        Mix_CloseAudio();
    }
#endif
    if( m_loaded.ttf )
    {
        // Shutdown ttf
        TTF_CloseFont( m_font_small );
        TTF_CloseFont( m_font_medium );
        TTF_CloseFont( m_font_big );
        TTF_CloseFont( m_font_biggest );
        TTF_Quit();
    }

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    if( m_loaded.sdl ) // Shutdown the SDL
        SDL_Quit();

    // Delete the setup structure
    delete m_setup;
    
    DEBUG( "Framework successfully quit!" );
}

//
// SetVideoMode
// Inits default video mode
//
void CFramework::SetVideoMode()
{
    const int scale = 2;

    if(!m_window)
        m_window = SDL_CreateWindow("Caption", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_setup->scr_w*scale, m_setup->scr_h*scale, m_setup->scr_flags );
    if(!m_renderer)
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(!m_screen)
        m_screen = SDL_CreateRGBSurfaceWithFormat(0, m_setup->scr_w, m_setup->scr_h, 32, SDL_PIXELFORMAT_RGBA8888);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(m_renderer, m_setup->scr_w, m_setup->scr_h);
}

//
// SetVideoMode
//
void CFramework::SetVideoMode( int w, int h )
{
    SDL_SetWindowMinimumSize(m_window, w, h);
    SDL_SetWindowMaximumSize(m_window, w, h);
}

//
// SetCaption
// Sets standard caption
//
void CFramework::SetCaption()
{
    // Set caption
    SDL_SetWindowTitle(m_window, m_setup->caption);
    
    // Disable cursor
    SDL_ShowCursor( SDL_FALSE );
}

//
// Error
// Handles expectable errors.
//
void CFramework::Error( Err_Msg msg )
{
    printf( "CTux_Error: " );
    switch( msg )
    {
        case ERR_SDL:
            printf( "Can't init SDL!" );
            break;
        case ERR_VID:
            printf( "Unable to setup video!" );
            break;
        case ERR_IMG:
            printf( "Image loading error!" );
            break;
        default:
            printf( "Unknown error!" );
            break;        
    }
    printf( "\n" );
}

//
// LoadImage
// So we don't have to write those lines for converting the image to the screen
// format every time we load an image from a file. Anyway converting our images
// to screen format is not necessary, but since we need any accessible piece of
// performance...
//
SDL_Texture * CFramework::LoadImage( const char *filename, bool withColorKey )
{
    SDL_Surface *temp = IMG_Load( filename );
    if( !temp )
    {
        Error( ERR_IMG );
        printf( "CTux_Msg: Can't load '%s'\n", filename );
        return NULL;
    }
    else
        printf( "CTux_Msg: Loaded image '%s'\n", filename );

    if(withColorKey)
        SDL_SetColorKey(temp, 1, SDL_MapRGB(temp->format, 255, 0, 255));

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, temp);
    SDL_FreeSurface(temp);

    return tex;
}

//
// GetScreen
// If you need to do some operations with the screen surface, that are not
// common enough to put them into the CFramework class.
//
SDL_Surface *CFramework::GetScreen()
{
    return m_screen;
}

//
// SetScreen
//
void CFramework::SetScreen( SDL_Surface *screen )
{
    m_screen = screen;
}

//
// GetLoaded
// To prevent you from using an incomplete initialized system.
//
SFramework_Loaded *CFramework::GetLoaded()
{
    return &m_loaded;
}

//
// GetSetup
// Helps you to find out in what way the technical stuff is initialized.
// For example use m_setup->fullscreen for special handling of the fs mode.
//
SFramework_Setup *CFramework::GetSetup()
{
    return m_setup;
}

//
// Lock
//
void CFramework::Lock( SDL_Surface *surf )
{
    if( SDL_MUSTLOCK( surf ) )
        SDL_LockSurface( surf );
}

//
// Unlock
//
void CFramework::Unlock( SDL_Surface *surf )
{
    if( SDL_MUSTLOCK( surf ) )
        SDL_UnlockSurface( surf );
}

//
// DrawPixel
//
void CFramework::DrawPixel( int x, int y, Color color )
{
    if( x < m_screen->w && x >= 0 &&
        y < m_screen->h && y >= 0 )
    {        
        Uint32 pixel = GetColorUint32( color );
        Uint8 *p = (Uint8 *)CFramework::m_screen->pixels + y * m_screen->pitch + x * 2;
        *(Uint16 *)p = pixel;
    }
}

//
// DrawGrid 
//
void CFramework::DrawGrid( int grid_w, int grid_h, int size_w, int size_h )
{
    int i, j;
    
    LockScreen();
    // Vertical lines
    for( i=0; i<size_h+1; i++ )
    {
        for( j=0; j<size_w+1; j+=grid_w )
            DrawPixel( j, i, COLOR_GREEN );
    }
        
    // Horizontal lines
    for( i=0; i<size_h+1; i+=grid_h )
    {
        for( j=0; j<size_w+1; j++ )
            DrawPixel( j, i, COLOR_GREEN );
    }
    UnlockScreen();
}

//
// Draw
// Most basic version. Takes only the surface that has to be blitted onto the 
// screen. Only use this, if you want to draw a background or a splash screen!
//
void CFramework::Draw(SDL_Texture *surf )
{
    SDL_RenderCopy(m_renderer, surf, nullptr, nullptr);
}
//
// Draw
// If you don't want to specify a rectangle for blitting
// Expects an x and y coord for the right position
//
void CFramework::Draw(SDL_Texture *surf, int x, int y )
{
    static SDL_Rect tmp_rect;
    tmp_rect.w = GetTextureWidth(surf);
    tmp_rect.h = GetTextureHeight(surf);
    tmp_rect.x = x;
    tmp_rect.y = y;

    Draw( surf, &tmp_rect );
}

//
// Draw
// This takes only the target rectangle
//
void CFramework::Draw(SDL_Texture *surf, SDL_Rect *target_rect )
{
    // Only draw when visible
    // Now we don't have to use this if-statement everytime we
    // could draw outside the screen...
    if( target_rect->x < m_screen->w &&
        target_rect->x + target_rect->w > 0 &&
        target_rect->y < m_screen->h &&
        target_rect->y + target_rect->h > 0 )
    {
        //SDL_BlitSurface( surf, NULL, m_screen, target_rect );
        SDL_RenderCopy(m_renderer, surf, nullptr, target_rect);
    }
}

//
// Draw
// Blits a surface onto the screen
// This version takes most parameters.
// See in the SDL library reference for mor informations...
//
void CFramework::Draw(SDL_Texture *surf, SDL_Rect *src_rect, SDL_Rect *target_rect )
{
    SDL_RenderCopy(m_renderer, surf, src_rect, target_rect);
}

//
// DrawText
//
void CFramework::DrawText( const char *text, int x, int y, Color color, Font_Size size )
{
    static SDL_Rect temp_rect;
    SDL_Texture *surf = DrawTextOnSurface( text, GetColorSDLColor( color ), size );
    temp_rect.x = x; temp_rect.y = y;

    int w, h;
    SDL_QueryTexture(surf, nullptr, nullptr, &w, &h);

    temp_rect.w = w; temp_rect.h = h;

    Draw( surf, &temp_rect );
    SDL_DestroyTexture(surf);
}

//
// DrawNumber
//
void CFramework::DrawNumber( int num, int x, int y, Color color, Font_Size size )
{
    // Convert number to char
	char *text = NumToText( num );
    DrawText( text, x, y, color, size );

	// Not anymore useful, so delete it!
	delete [] text;
}

//
// DrawTextOnSurface
//
SDL_Texture *CFramework::DrawTextOnSurface( const char *text, SDL_Color color, Font_Size size )
{
    SDL_Surface *surf;
    
    switch( size )
    {
        case F_SIZE_SMALL:
            surf = TTF_RenderText_Solid( m_font_small, text, color );
            break;
        case F_SIZE_MEDIUM:
            surf = TTF_RenderText_Solid( m_font_medium, text, color );
            break;
        case F_SIZE_BIG:
            surf = TTF_RenderText_Solid( m_font_big, text, color );
            break;
        default:
        case F_SIZE_BIGGEST:
            surf = TTF_RenderText_Solid( m_font_biggest, text, color );
            break;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_framework->GetRenderer(), surf);
    SDL_FreeSurface(surf);
    
    return tex;
}

//
// DrawShadowText
//
void CFramework::DrawShadowText( const char *text, int x, int y, Color shadow_color,
                                    Color color, Font_Size size )
{
    DrawText( text, x+2, y+2, shadow_color, size );
    DrawText( text, x, y, color, size );
}

//
// DrawShadowText
//
void CFramework::DrawShadowNumber( int num, int x, int y, Color shadow_color,
                                    Color color, Font_Size size )
{
    DrawNumber( num, x+2, y+2, shadow_color, size );
    DrawNumber( num, x, y, color, size );
}

//
// GetTicks()
// Returns the time in miliseconds since the application has been started
//
Uint32 CFramework::GetTicks()
{
    return SDL_GetTicks();
}

//
// SetAlpha
// To make a surface transparent
//
void CFramework::SetAlpha(SDL_Texture *surf, Uint8 alpha )
{
    SDL_SetTextureAlphaMod(surf, (int)ceil(alpha*1.5));
}

//
// SetColorKey
//
void CFramework::SetColorKey(SDL_Texture *surf, Color colorkey )
{
    //SDL_SetColorKey( surf, SDL_SRCCOLORKEY, GetColorUint32( colorkey ) );
}

//
// FillRect
// Fill a specified with a color
// More or less a wrapper around the SDL to make the game
// more portable. SDL specific types should be replaced also...
//
void CFramework::FillRect( SDL_Surface *surf, SDL_Rect *rect, Color color )
{
    SDL_FillRect( surf, rect, GetColorUint32( color ) );
}

//
// GetColor
//
Uint32 CFramework::GetColorUint32( Color color )
{
    switch( color )
    {
        default:
        case COLOR_MAGENTA:
            return m_uint32_colors.magenta;
            break;
        case COLOR_BLACK:
            return m_uint32_colors.black;
            break;
        case COLOR_WHITE:
            return m_uint32_colors.white;
            break;
        case COLOR_RED:
            return m_uint32_colors.red;
            break;
        case COLOR_GREEN:
            return m_uint32_colors.green;
            break;
        case COLOR_BLUE:
            return m_uint32_colors.blue;
            break;
        case COLOR_BRIGHT_BLUE:
            return m_uint32_colors.bright_blue;
            break;
        case COLOR_GRAY:
            return m_uint32_colors.gray;
            break;
        case COLOR_YELLOW:
            return m_uint32_colors.yellow;
            break;
    }
}

//
// GetColor
//
SDL_Color CFramework::GetColorSDLColor( Color color )
{
    switch( color )
    {
        default:
        case COLOR_MAGENTA:
            return m_sdl_colors.magenta;
            break;
        case COLOR_BLACK:
            return m_sdl_colors.black;
            break;
        case COLOR_WHITE:
            return m_sdl_colors.white;
            break;
        case COLOR_RED:
            return m_sdl_colors.red;
            break;
        case COLOR_GREEN:
            return m_sdl_colors.green;
            break;
        case COLOR_BLUE:
            return m_sdl_colors.blue;
            break;
        case COLOR_BRIGHT_BLUE:
            return m_sdl_colors.bright_blue;
            break;
        case COLOR_GRAY:
            return m_sdl_colors.gray;
            break;
        case COLOR_YELLOW:
            return m_sdl_colors.yellow;
            break;
    }
}

//
// Coll
// Detect wheter two rectangles collide
//
bool CFramework::Coll( SDL_Rect *rect1, SDL_Rect *rect2 )
{
    return(
        // horizontal
        rect1->x + rect1->w > rect2->x &&
        rect1->x < rect2->x + rect2->w &&
        // vertical
        rect1->y + rect1->h > rect2->y &&
        rect1->y < rect2->y + rect2->h );
}

//
// ReverseDirection
//
void CFramework::ReverseDirection( Direction *dir )
{
    if( *dir == DIR_LEFT )
        *dir = DIR_RIGHT;
    else
        *dir = DIR_LEFT;
}

//
// MakeScreenshot
//
void CFramework::MakeScreenshot( const char *filename )
{
    SDL_SaveBMP( m_screen, filename );
}

//
// Fullscreen
//
void CFramework::Fullscreen()
{
    // Only if not already fullscreen
    if( !m_setup->fullscreen )
    {
        /*m_setup->scr_flags += SDL_WINDOW_FULLSCREEN;
        SetVideoMode();*/
        /*m_setup->fullscreen = true;
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);*/
    }
}

//
// Windowed
//
void CFramework::Windowed()
{
    // Only if not already windowed
    if( m_setup->fullscreen )
    {
        m_setup->fullscreen = false;
        /*m_setup->scr_flags -= SDL_WINDOW_BORDERLESS;
        SetVideoMode();
        SDL_RenderPresent(m_renderer);*/
        //SDL_SetWindowFullscreen(m_window, SDL_WINDOW_OPENGL);
    }
}
