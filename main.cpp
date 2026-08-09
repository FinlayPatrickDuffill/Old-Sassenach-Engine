/*

    This file is the CLIENT program entry point -
    Typical program include hierarchy is currently as follows:
    [do this later]

    Acknowledgements:
    Lazy Foo Productions - OpenGL, Loading a Texture - https://lazyfoo.net/tutorials/OpenGL/06_loading_a_texture/index.php
    Sam Lantinga - SDL_net example chat client - https://github.com/SDL-mirror/SDL_net/blob/master/chat.cpp
    Graham Trott - Dynamic arrays in C++ - https://dev.to/gtanyware/dynamic-arrays-in-c-24oj

*/

// This include hierarchy is mandatory
#include "LUtil.h"
#include "LTexture.h"
#include "LEvents.h"
#include "Network.h"
#include "LMapLoading.h"
#include "main.h"

//SDL context
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_thread.h>
#include <SDL_net.h>

//ObjectArrays for map display and loading -
ObjectArray* active_chunk = new ObjectArray();
ObjectArray* peripheral_chunk_N = new ObjectArray();
ObjectArray* peripheral_chunk_NE = new ObjectArray();
ObjectArray* peripheral_chunk_ES = new ObjectArray();
ObjectArray* peripheral_chunk_E = new ObjectArray();
ObjectArray* peripheral_chunk_W = new ObjectArray();
ObjectArray* peripheral_chunk_WN = new ObjectArray();
ObjectArray* peripheral_chunk_S = new ObjectArray();
ObjectArray* peripheral_chunk_SW = new ObjectArray();

int main( int argc, char* args[] )
{
    // Initializing the game window
    if( !initialize_graphical_window() )
    {
        printf( "Unable to initialize graphics library!\n" );
        return CLOSE_PROGRAM_MACRO_ONE;
    }
    printf( "Able to initialize graphics library...\n" );

    // SDL network initialization steps
    // Initialize SDL_net
    if ( SDLNet_Init() < 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't initialize net: %s\n",
                     SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }
    // Check keyboard state
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    gameworld_constants_logic = init_constants(gameworld_constants_logic);
    // Information to read from a configuration file that stores data about the player's position on the map
    config_file_terminologies config_file_terminology;
    const char* name_of_debug_record_file;
    name_of_debug_record_file = "player_data_record";
    // asset loading section of the spaghetti
    // refactor this v
    initial_texture_loading();
    // textures for the pseudo-terminal emulator
    gameworld_constants_logic = load_terminal_alphabet(gameworld_constants_logic);
    dynamic_screenprint_function(150.f, 160.f, gameworld_constants_logic, "there is a connection attempt");
    SDL_GL_SwapWindow( gWindow );
    // soon obsolete hardcoded_network_test_function
    soon_obsolete_hardcoded_network_test_function(gameworld_constants_logic, gWindow);
    main_menu_state_systems = main_menu_state_idle;
    game_state_systems = game_state_on;
    // the start of the game state machine
    while( game_state_systems != game_state_off )
	{
    // a state machine for the menu; this isn't the way to do it
    while ( main_menu_state_systems != main_menu_state_begin_game )
		{
		    keyboardState = SDL_GetKeyboardState(NULL);
    		rendering_controls = control_handling_on_menu(gameworld_constants_logic, e, keyboardState, rendering_controls, gWindow, renderer);
		}
		rendering_controls.quitting_game = false;
	dynamic_screenprint_function(150.f, 120.f, gameworld_constants_logic, "loading your map");
	SDL_GL_SwapWindow( gWindow );
    // populates the user's perspective when loading the game, the single player implementation
    debug_user_perspective = parse_player_data_from_record_particular(name_of_debug_record_file, "Admin", debug_user_perspective, config_file_terminology);
    debug_user_perspective.angle_reference_x = 0 - debug_user_perspective.unlabelled_user_perspective_data[1];
    debug_user_perspective.Coord_y = 0 - debug_user_perspective.unlabelled_user_perspective_data[2];
    debug_user_perspective.Coord_z = 0.f - debug_user_perspective.unlabelled_user_perspective_data[3];
    printf("Coord z %f\n ",debug_user_perspective.Coord_z);
    printf("z in perspective %f\n",debug_user_perspective.Coord_z);
    // default frame delay
    frame_delay = FRAME_DELAY_MACRO;
    // create a renderer
    renderer = SDL_CreateRenderer(gWindow, SDL_VIDEO_RENDER_OGL, 0);
    rendering_chunk_setup(gameworld_constants_logic, active_chunk,
                          peripheral_chunk_WN,
                          peripheral_chunk_N, peripheral_chunk_NE,
                           peripheral_chunk_S,  peripheral_chunk_SW,
                            peripheral_chunk_E, peripheral_chunk_ES, peripheral_chunk_W);
    rendering_controls.skipframe = 0;
    // bad refactoring - this function should execute if the map didn't load correctly
    reloading_upon_memory_leak("terrain,0,0,0.txt",gameworld_constants_logic, gWindow, active_chunk);
    reloading_upon_memory_leak("terrain,-1,1,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_WN);
    reloading_upon_memory_leak("terrain,0,1,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_N);
    reloading_upon_memory_leak("terrain,1,1,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_NE);
    reloading_upon_memory_leak("terrain,0,-1,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_S);
    reloading_upon_memory_leak("terrain,-1,-1,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_SW);
    reloading_upon_memory_leak("terrain,1,0,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_E);
    reloading_upon_memory_leak("terrain,1,-1,0.TXT",gameworld_constants_logic, gWindow, peripheral_chunk_ES);
    reloading_upon_memory_leak("terrain,-1,0,0.txt",gameworld_constants_logic, gWindow, peripheral_chunk_W);
    // game logic loop while you're in the game world
    while( !rendering_controls.quitting_game )
		{
			keyboardState = SDL_GetKeyboardState(NULL);
			// this handles user input
			rendering_controls = control_handling_on_map(gameworld_constants_logic, e, keyboardState, rendering_controls, gWindow, renderer);
			// refresh the screen
            glClear( GL_COLOR_BUFFER_BIT );
            // handle the controls etcetera
			debug_user_perspective = main_rendering_logic_loop(gameworld_constants_logic, active_chunk, debug_user_perspective, e, keyboardState, rendering_controls, gWindow, renderer);
            // there's probably an easier way to do this
            render_userrelation(peripheral_chunk_S, debug_user_perspective, renderer, gWindow, rendering_controls);
            render_userrelation(peripheral_chunk_SW, debug_user_perspective, renderer, gWindow, rendering_controls);
            render_userrelation(peripheral_chunk_WN, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(peripheral_chunk_N, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(peripheral_chunk_NE, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(peripheral_chunk_E, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(peripheral_chunk_ES, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(peripheral_chunk_W, debug_user_perspective, renderer, gWindow, rendering_controls);
			render_userrelation(active_chunk, debug_user_perspective, renderer, gWindow, rendering_controls);
            //screen dialogue
            primitive_screenprint_function(gameworld_constants_logic, "map view");
            reset_controls(rendering_controls);
            SDL_Delay(frame_delay);
            SDL_GL_SwapWindow( gWindow );
        }
	}
    SDLNet_Quit();
    return 0;
}
