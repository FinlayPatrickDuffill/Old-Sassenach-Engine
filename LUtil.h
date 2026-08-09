#ifndef LUTIL_H
#define LUTIL_H

#define CLOSE_PROGRAM_MACRO_ONE 1
#define CLOSE_PROGRAM_MACRO_TWO 2

#include "LOpenGL.h"
#include "LTexture.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_events.h>
#include "LMapLoading.h"

typedef struct {
    int cache_flag_int, cache_data_int;
    bool cache_flag_bool, cache_data_bool;
    char cache_flag_char, cache_data_char;
} debug_cache_flags;

//Debugging cache for debugging
debug_cache_flags write_to_debug_cache(debug_cache_flags* debug_cache_flag_bunch);

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();

bool loadMedia(int image_identity);

bool loadMedia_particular(int image_identity, const char* filename);

void update();

void render(SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls );

void render_userrelation(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls );

void render_userrelation_N(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls);
/*
I don't know why I'm creating a separate function
*/

void initial_texture_loading();

gameworld_constants load_terminal_alphabet(gameworld_constants gameworld_constants_logic);

#endif
