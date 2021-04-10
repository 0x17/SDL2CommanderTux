/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// See Readme.txt for more informations!

// CommanderTux.cpp
// Main project file

#include "Framework.h"
#include "Level.h"
#include "Splash.h"
#include "Game.h"
#include "Editor.h"
#include "Duel.h"

//
// main
// Entrypoint
//
int main( int argc, char *argv[] )
{    
    bool stop; // for --help
    bool done = false;

    // Setup structure
    SFramework_Setup setup;
    
    // States
    CSplash *splash;    
    CGame *game;
    CDuel *duel;
    CEditor *editor;
    
    // Cmd line stuff
    setup = ParseArgs( argc, argv, &stop );
    if( stop )
        return 0;
    
    // Default msgs
    DEBUG( "== CommanderTux ==" );    
    DEBUG( "== Starting... ==" );
    SHOW_VERSION();
    DEBUG( "http://commandertux.sourceforge.net" );
      
    setup.caption = "CommanderTux";
    
    // Init framework
    new CFramework( &setup );
    
    // The splash state is not in the loop of states
    // Because it is only shown once the game is started!
    splash = new CSplash();    
    if( !splash->Loop() ) // If fast exit
    {
        delete splash;
        delete g_framework;
        return 0;
    }
    delete splash;
    
    // Main loop between the states
    while( !done )
    {
        // Start the different game states
        if( !setup.editor && !setup.duel ) // Normal mode
        {            
            game = new CGame( &setup );
            if( !game->Loop() ) // If fast exit
            {
                delete game;
                delete g_framework;
                return 0;
            }
            done = game->GetAllDone();
            delete game;
        }
        
        if( setup.duel ) // Duel mode
        {
            duel = new CDuel();
            if( !duel->Loop() ) // If fast exit
            {
                delete duel;
                delete g_framework;
                return 0;
            }
            delete duel;
            setup.duel = false;
        }
        
        if( setup.editor ) // Leveleditor
        {            
            editor = new CEditor();
            if( !editor->Loop() ) // If fast exit
            {
                delete editor;
                delete g_framework;
                return 0;
            }
            delete editor;
            setup.editor = false;
        }
    }

    // Shutdown framework
    delete g_framework;
    
    DEBUG( "== Shutdown ==" );
    return 0;
}
