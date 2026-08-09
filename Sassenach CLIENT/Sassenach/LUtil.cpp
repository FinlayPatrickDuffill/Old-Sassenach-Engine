#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"
#include <SDL.h>
#include <SDL_events.h>
#include <iostream>
#include <fstream>

int metric, image_memory_position = 0;

//File loaded texture
LTexture gLoadedTexture;

bool initGL()
{
    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    //Enable texturing
    glEnable( GL_TEXTURE_2D );
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    //Initialize DevIL
    ilInit();
    ilClearColour( 255, 255, 255, 000 );
    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }
    return true;
}

bool loadMedia(int image_identity)
{
    //Load texture
    if( !gLoadedTexture.loadTextureFromFile( "assets/floor.png", 0 ) )
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }
    return true;
}

//Load a particular media file is the idea
bool loadMedia_particular(int image_identity, const char* filename)
{
    // printf("loadMedia_particular called -  image identity is: %d \n", image_identity);
    //Load texture
    if( !gLoadedTexture.loadTextureFromFile( filename, image_identity ) )
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }
    return true;
}

void update()
{

}

trigonometry_rendering trig;

void render(SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry( trig.angle_flag, x, y, trig );
}

void render_userrelation(ObjectArray *chunk, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    // set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry_userrelation(rendering_controls, chunk, user_perspective, trig.angle_flag, x, y, trig );
    //Update screen
}

void render_userrelation_N(ObjectArray *chunk, ObjectArray *peripheral_chunk_N, user_perspective user_perspective, SDL_Renderer* renderer, SDL_Window* window, controls rendering_controls)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 2.f;
    //Rendering a floor
    // set_angle(rendering_controls.angle_direction);
    gLoadedTexture.render_trigonometry_userrelation(rendering_controls, chunk, user_perspective, trig.angle_flag, x, y, trig );
    //Update screen
}

void initial_texture_loading()
{
       if( !loadMedia_particular(1, "assets/floor.png"))
    {
        printf( "Couldn't load a texture\n" );
    }
    else
    {
        image_memory_position++;
        printf("Floor texture loaded\n");
    }

    // initial asset loading
    if( !loadMedia_particular(2, "assets/floor2.png") )
    {
        printf( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        printf("Floor texture loaded\n");
    }
    if( !loadMedia_particular(3, "assets/floor3.png") )
    {
        printf( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        printf("Floor texture loaded\n");
    }
    if( !loadMedia_particular(4, "assets/player.png") )
    {
        printf( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        printf("Player texture loaded\n");
    }
        if( !loadMedia_particular(5, "assets/floor4.png") )
    {
        printf( "Couldn't load texture\n" );
    }
    else
    {
        image_memory_position++;
        printf("Floor texture loaded\n");
    }
}

gameworld_constants load_terminal_alphabet(gameworld_constants gameworld_constants_logic)
{
std::string terminal_alphabet_filepath = "assets/alphabet/letter_";
const char* terminal_alphabet_filepath_cchar = "assets/alphabet/letter_x";
char terminal_alphabet_appended_letter = 'a';
// printf("%s \n", player_datum_begin);
for (int i=0;i!=26;i++)
    {
        terminal_alphabet_filepath= "assets/alphabet/letter_";
        terminal_alphabet_filepath += terminal_alphabet_appended_letter;
        terminal_alphabet_filepath += ".png";
        terminal_alphabet_appended_letter++;
        terminal_alphabet_filepath_cchar = terminal_alphabet_filepath.c_str();
        if( !loadMedia_particular(6, terminal_alphabet_filepath_cchar) )
            {

            }
        else
            {
                image_memory_position++;
            }
        }
        // inform DevIL where the end of the IDs on the heap loaded into memory to represent an alphabet is
        gameworld_constants_logic.termination_of_loaded_alphabet_in_memory = image_memory_position;
        // update the gameworld constants
    return gameworld_constants_logic;
}

