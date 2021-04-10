/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// State.h

#ifndef STATE_H
#define STATE_H

#include <SDL.h>
#include "Framework.h"

// Input stuff
struct SReleased
{
    bool key_return, key_escape;
    bool key_left, key_right;
    bool key_up, key_down;
    bool key_s, key_p; // for the editor
    bool key_2, key_3, key_4;
    bool key_space;
    
    SReleased()
    {
        key_return = key_escape = false;
        key_left = key_right = false;
        key_up = key_down = false;
        key_s = key_p = false;
        key_2 = key_3 = key_4 = false;
        key_space = false;
    }
};

// TODO: time based motion
// prevent the game from running to fast on better machines
#define MAX_FPS     25

// Every state in the game has to derive from this class
class CState
{
    public:
        // Constructor
        CState()
        { m_max_fps = MAX_FPS; }
        
        // Contains to fundamental lines for input processing
        // Every derived class should call this
        virtual void Input()
        {
            SDL_PumpEvents();
            m_keys = SDL_GetKeyboardState( nullptr );
            
            if( m_keys[SDL_SCANCODE_F3] )
                g_framework->MakeScreenshot("../screenshots/screenshot.bmp");
        }
        
        virtual void Update() = 0;
        virtual void Draw() = 0;
        
        // The main loop // shouldn't be modified by any derived class
        bool Loop() // returns false if quit
        {
            // So we never use an incomplete initialized framework
            if( g_framework->GetLoaded()->everything )
            {
                Uint32 old_time;
                m_done = false;
                while( !m_done )
                {
                    old_time = g_framework->GetTicks();
                    // Event handling... only interesting in windowed mode
                    while( SDL_PollEvent( &m_event ) )
                    {
                        if( m_event.type == SDL_QUIT )
                            return false;
                    }
                    
                    Input();
                    Update();
                    Draw();

                    SDL_RenderPresent( g_framework->GetRenderer() );

                    while( (g_framework->GetTicks() - old_time) < 1000 / m_max_fps );
                }
                return true;
            }
			return false;
        }
        
        bool GetDone() { return m_done; }
    protected:
        const Uint8 *m_keys;
        bool m_done;
        SDL_Event m_event;
        Uint32 m_max_fps;
};

#endif
