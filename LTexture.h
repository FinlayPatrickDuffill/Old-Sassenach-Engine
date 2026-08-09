#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "LOpenGL.h"
#include <stdio.h>
#include <string>
#include <glm.hpp>
#include <trigonometric.hpp>
#include "LMapLoading.h"

// optimization idea
#define flag_not_peripheral 0
#define peripheral_north 1
#define peripheral_northeast 2
#define peripheral_east 3
#define peripheral_southeast 4
#define peripheral_south 5
#define peripheral_southwest 6
#define peripheral_west 7
#define peripheral_northwest 8

#define array_parameters_for_position 3

#define number_of_config_file_terminologies 7

typedef struct gameworld_constants{
        int chunk_row_width,
        chunk_zbound,
        chunk_grid_width,
        termination_of_loaded_alphabet_in_memory ;
        bool connection_status;
        ObjectArray* constant_flags;
} gameworld_constants;

typedef struct {
    float player_position_in_rendering_bounds[array_parameters_for_position];
    float player_position_in_world[array_parameters_for_position];
} player_information;

typedef struct {
  float vertice_N,vertice_E,vertice_S,vertice_W;
  float vertice_Ny,vertice_Ey,vertice_Sy,vertice_Wy;
  float vertice_Nx,vertice_Ex,vertice_Sx,vertice_Wx;
  float vertice_Nytotal,vertice_Eytotal,vertice_Sytotal,vertice_Wytotal;
  float vertice_Nxtotal,vertice_Extotal,vertice_Sxtotal,vertice_Wxtotal;
  float tilewidth, tileheight;
  float angle, angle_flag;
} trigonometry_rendering ;

typedef struct {
    float angle_direction, quitting_game, movement_test;
    int skipframe;
    int FOV = 10;
} controls;

void reset_controls(controls rendering_controls);

  // The client user's perspective
typedef struct {
  int angle_reference_x;
  float initial_visible_portion[array_parameters_for_position];
  // Position (?) information
  float Coord_x, Player, Coord_z;
  int Coord_y;
  float rendering_position_x, rendering_position_y;
  // User flags
  ObjectArray* user_flags;
  // float array, unlabelled user perspective data
  float unlabelled_user_perspective_data[20];
} user_perspective;

typedef struct {
  const char* item_of_terminology[number_of_config_file_terminologies];
} config_file_terminologies;

// part of a trigonometry test that renders an isometric plane which responds to user input
trigonometry_rendering populate_trig(trigonometry_rendering);

//load hardcoded terminology to parse a config file
config_file_terminologies set_default_config_file_terminologies(config_file_terminologies file_terminologies);
//load different terminology with different intentions
config_file_terminologies load_custom_config_file_terminologies(const char* item_of_terminology);

//the initial user perspective from a player loading into the game, derived from a config file, hardcoded values or manual input
user_perspective initial_user_perspective(user_perspective* user_perspective, float initial_angle, float initial_visible_portion[array_parameters_for_position], player_information player_information);


class LTexture
{
public:
    LTexture();

    ~LTexture();

    bool loadTextureFromFile( std::string path, int image_identity );

    bool loadTextureFromPixels32( int image_identity, GLuint* pixels, GLuint width, GLuint height );

    void freeTexture();

    void render_trigonometry_userrelation(controls rendering_controls, ObjectArray *chunk, user_perspective user_perspective, float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig );

    void render_peripheral_chunk(int terrain_index, int elevation, int elevation_increment, user_perspective user_perspective, ObjectArray *peripheral_chunk, int visibletilesheight, int square_chunk_formula, GLuint mTextureID, trigonometry_rendering trig);

    void renderisometricgrid_userrelation(int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective);

    void renderisometricblock_userrelation(int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective);

    void renderisometricfloor(float Nv1, float Nv2, float Ev1, float Ev2, float Sv1, float Sv2, float Wv1, float Wv2);

    void render_trigonometry( float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig);

    GLuint getTextureID();

    GLuint textureWidth();

    GLuint textureHeight();

private:
    GLuint mTextureID;

    GLuint mTextureWidth;
    GLuint mTextureHeight;
};

void set_angle(float angle_flag);

void primitive_screenprint_function(gameworld_constants gameworld_constants_logic, string printing_string);

void dynamic_screenprint_function(float x, float y, gameworld_constants gameworld_constants_logic, string printing_string);

void rendering_chunk_setup(gameworld_constants gameworld_constants_logic,
                            ObjectArray* active_chunk,
                            ObjectArray* peripheral_chunk_WN,
                           ObjectArray* peripheral_chunk_N,
                            ObjectArray* peripheral_chunk_NE, ObjectArray* peripheral_chunk_S,
                            ObjectArray* peripheral_chunk_SW,
                              ObjectArray* peripheral_chunk_E, ObjectArray* peripheral_chunk_ES,
                              ObjectArray* peripheral_chunk_W);

void reloading_upon_memory_leak(const char* filename, gameworld_constants gameworld_constants_logic, SDL_Window* gWindow, ObjectArray* active_chunk);

void reloading_upon_memory_leak(const char* filename, gameworld_constants gameworld_constants_logic, SDL_Window* gWindow, ObjectArray* active_chunk);

gameworld_constants init_constants(gameworld_constants constants_to_init);

#endif
