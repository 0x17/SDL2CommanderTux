/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Framework.h
// This similiar to an engine

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#ifndef NO_SOUND
#include <SDL_mixer.h>
#endif

// this is only for old compilers that don't understand the full standard
// VS6 users have to enable this!!!
#define CSTYLE 0

#if CSTYLE == 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for memcpy!
#include <time.h> // time() for srand
#include <math.h>

#else

#include <cstdlib>
#include <cstdio>
#include <cstring> // for memcpy!
#include <ctime> // time() for srand
#include <cmath>
using namespace std;
// VS6 users have to enable CSTYLE!

#endif

#include "Defines.h"
#include "Filenames.h"
#include "LinkedList.h"

struct SFramework_Setup;
SFramework_Setup ParseArgs( int argc, char *argv[], bool *stop );

struct SFramework_Setup
{
    bool doublebuf, fullscreen;
    int scr_w, scr_h, scr_bpp;
    Uint32 scr_flags;
    const char *caption;
    bool editor, duel;
    
    SFramework_Setup()
    {
        scr_w = SCR_W;
        scr_h = SCR_H;
        scr_bpp = 16;
        scr_flags = SDL_WINDOW_OPENGL;
        doublebuf = true;
        fullscreen = false;
        caption = "Framework";
        editor = duel = false;
    } 
};

// Loaded structure, so we only shutdown things that are already loaded!
struct SFramework_Loaded
{
    bool sdl, vid, snd, ttf;
    bool everything;
    
    SFramework_Loaded()
    { sdl = vid = snd = ttf = everything = false; }
};

// Error messages
typedef enum
{
    ERR_SDL,
    ERR_VID,
    ERR_SND,
    ERR_IMG
} Err_Msg;

// Font sizes
typedef enum
{
    F_SIZE_SMALL,
    F_SIZE_MEDIUM,
    F_SIZE_BIG,
    F_SIZE_BIGGEST
} Font_Size;

// SDL colors
struct SDL_Colors
{
    SDL_Color magenta, black, white;
    SDL_Color red, green, blue;
    SDL_Color bright_blue, gray;
    SDL_Color yellow;
};

// Unsigned int 32bit colors
struct Uint32_Colors
{
    Uint32 magenta, black, white;
    Uint32 red, green, blue;
    Uint32 bright_blue, gray;
    Uint32 yellow;
};

// Colors
typedef enum
{
    COLOR_MAGENTA,
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_BRIGHT_BLUE,
    COLOR_GRAY,
    COLOR_YELLOW
} Color;

class CFramework
{
    public:
        CFramework( SFramework_Setup *setup = NULL );
        ~CFramework();
        
        void SetVideoMode();
        void SetVideoMode( int w, int h );
        void SetCaption();
        
        void Error( Err_Msg msg );
        SDL_Texture * LoadImage(const char *filename, bool withColorKey = true );
        
        SDL_Surface *GetScreen();
        void SetScreen( SDL_Surface *screen );

        SDL_Renderer *GetRenderer() {
            return m_renderer;
        }

        SFramework_Loaded *GetLoaded();
        SFramework_Setup *GetSetup();
        
        void Lock( SDL_Surface *surf );
        void Unlock( SDL_Surface *surf );
        
        void LockScreen() { Lock( m_screen ); }
        void UnlockScreen() { Unlock( m_screen ); }
        
        void DrawPixel( int x, int y, Color color );
        void DrawGrid( int grid_w, int grid_h, int size_w, int size_h );
        void Draw(SDL_Texture *surf );
        void Draw(SDL_Texture *surf, int x, int y );
        void Draw(SDL_Texture *surf, SDL_Rect *target_rect );
        void Draw(SDL_Texture *surf, SDL_Rect *src_rect, SDL_Rect *target_rect );
        void DrawText( const char *text, int x, int y, Color color, Font_Size size );
        SDL_Texture *DrawTextOnSurface( const char *text, SDL_Color color, Font_Size size );
        void DrawNumber( int num, int x, int y, Color color, Font_Size size );
        void DrawShadowText( const char *text, int x, int y, Color shadow_color, Color color, Font_Size size );
        void DrawShadowNumber( int num, int x, int y, Color shadow_color, Color color, Font_Size size );
        
        Uint32 GetTicks();
        
        void SetAlpha(SDL_Texture *surf, Uint8 alpha );
        void SetColorKey(SDL_Texture *surf, Color colorkey );
        void FillRect( SDL_Surface *surf, SDL_Rect *rect, Color color );
        
        Uint32 GetColorUint32( Color color );
        SDL_Color GetColorSDLColor( Color color );
        
        bool Coll( SDL_Rect *rect1, SDL_Rect *rect2 );
        
        void ReverseDirection( Direction *dir );
        void ReverseBool( bool *var )
        {
            if( *var )
                *var = false;
            else
                *var = true;
        }
        
        void MakeScreenshot( const char *filename );
        
		// Never forget to delete the given buffer
		// after you are done
        char *NumToText( int num )
        {
            char *buffer = new char [256];
            sprintf( buffer, "%d", num );
			return buffer;
        }
        
        void Fullscreen();
        void Windowed();
        
        void ShowCursor( bool value )
        {
            if( value )
                SDL_ShowCursor( SDL_ENABLE );
            else
                SDL_ShowCursor( SDL_DISABLE );
        }
        
        void SetCaption( const char *caption )
        {
            m_setup->caption = caption;
            SDL_SetWindowTitle(m_window, caption);
        }

        int GetTextureWidth(SDL_Texture *tex) const {
            int w;
            SDL_QueryTexture(tex, nullptr, nullptr, &w, nullptr);
            return w;
        }

        int GetTextureHeight(SDL_Texture *tex) const {
            int h;
            SDL_QueryTexture(tex, nullptr, nullptr, nullptr, &h);
            return h;
        }

    private:
        SDL_Renderer *m_renderer;
        SDL_Window *m_window;

        SDL_Surface *m_screen;

        TTF_Font *m_font_small, *m_font_medium, *m_font_big, *m_font_biggest;
        
        SFramework_Setup *m_setup;
        SFramework_Loaded m_loaded;
        
        SDL_Colors m_sdl_colors;
        Uint32_Colors m_uint32_colors;
};

extern CFramework *g_framework;

#endif
