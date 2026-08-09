#include "LTexture.h"
#include "LMapLoading.h"
#include <IL/IL.h>
#include <glm.hpp>
#include <SDL.h>

#define IDDebugTerrainWall 5
#define IDDebugTerrain 2
#define IDTerrainInvert 1
#define blockelevation 25

#define user_debug_sprite_index 4
//mirage is 1
#define mirage 1

bool textures_generated;
int terrain_index;
float trig_formula_a, trig_formula_b, trig_formula_c, trig_formula_d ;
int texval;
string terrain_value;
bool skip = false;

// float i stores / used to store the angle of the isometric perspective
//there's only one screen, I probably don't have to change this right
float i, angle_flag;
int elevation, elevation_increment;

typedef struct player{
    int coord_x, coord_y, coord_z;
    ObjectArray* player_flags;
}   player;

//load hardcoded terminology to parse a config file
config_file_terminologies set_default_config_file_terminologies(config_file_terminologies file_terminologies){
    file_terminologies.item_of_terminology[0] = "Player";
    file_terminologies.item_of_terminology[1] = "Coord_x";
    file_terminologies.item_of_terminology[2] = "Coord_y";
    file_terminologies.item_of_terminology[3] = "Coord_z";
    return file_terminologies;
}

LTexture::LTexture()
{
    //Initialize texture ID
    mTextureID = 0;
    //printf("LTexture called Texture identity is: %d \n", mTextureID);
    textures_generated = false;
    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture()
{
    //Free texture data if needed
    freeTexture();
}

void render_flat_quad(GLuint mTextureID, float Nv1, float Nv2, float Ev1, float Ev2, float Sv1, float Sv2, float Wv1, float Wv2){
        glBindTexture( GL_TEXTURE_2D, mTextureID );
        glColor3ub(210, 210, 255);
        glEnable( GL_BLEND );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 0.f ); glVertex2f(Nv1, Nv2);
        glTexCoord2f( 1.f, 0.f ); glVertex2f(Ev1, Ev2);
        glTexCoord2f( 1.f, 1.f ); glVertex2f(Sv1, Sv2);
        glTexCoord2f( 0.f, 1.f ); glVertex2f(Wv1, Wv2);
        glEnd();
}
void render_flat_sprite(GLuint mTextureID){
        glBindTexture( GL_TEXTURE_2D, mTextureID );
        glColor3ub(255, 255, 255);
        glEnable( GL_BLEND );
        glBegin( GL_QUADS );
        glTexCoord2f( 1.f, 1.f ); glVertex2f(0.f, 0.f);
        glTexCoord2f( 2.f, 1.f ); glVertex2f(29.f, 0.f);
        glTexCoord2f( 2.f, 2.f ); glVertex2f(29.f, 29.f);
        glTexCoord2f( 1.f, 2.f ); glVertex2f(0.f, 29.f);
        glEnd();
        glColor3ub(255, 255, 255);
}

bool LTexture::loadTextureFromFile( std::string path, int image_identity )
{
    //Texture loading success
    bool textureLoaded = false;

//    printf("LoadTextureFromFile called -  Texture identity is: %d \n", mTextureID);

    //Generate and set current image ID
    ILuint imgID = image_identity;
    ilGenImages( image_identity, &imgID );
    ilBindImage( imgID );

//    printf("LoadTextureFromFile called -  Image identity is: %d \n", imgID);

    //Load image
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( image_identity, (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !textureLoaded )
    {
//        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool LTexture::loadTextureFromPixels32( int image_identity, GLuint* pixels, GLuint width, GLuint height )
{
//    printf("Image identity is: %d \n", image_identity);
    //Free texture if it exists
    //Get texture dimensions
    mTextureWidth = width;
    mTextureHeight = height;
    glGenTextures( 1, &mTextureID );
    textures_generated = true;
    //Need to create more named textures? X no that's not how that works
    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );
    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
//        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }
    return true;
}

void LTexture::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
    }
    mTextureWidth = 0;
    mTextureHeight = 0;
}

GLuint terrain_buffer(ObjectArray *chunk, GLuint mTextureID){

        terrain_value = (((Terrain*)chunk->get(terrain_index))->getName());
        //DEBUG_TERRAIN_WALL
        if (terrain_value == "DEBUG_TERRAIN_WALL")
        {
           mTextureID = IDDebugTerrainWall;
           skip = false;
        }
        //DEBUG_TERRAIN
        if (terrain_value == "DEBUG_TERRAIN")
        {
           mTextureID = IDDebugTerrain;
           skip = false;
        }
        if (terrain_value == "AIR")
        {
           skip = true;
        }
        //DEBUG_TERRAIN_INVERT
        if (terrain_value == "DEBUG_TERRAIN_INVERT")
        {
           mTextureID = IDTerrainInvert;
           skip = false;
        }
        return mTextureID;
}

void LTexture::renderisometricblock_userrelation( int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective)
{

    //Set texture ID
    if (skip == false)
    {
        if (mTextureID == IDDebugTerrainWall)
        {
            trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
            trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
            trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
            trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
            glTranslatef(250 + ((trig_formula_a + trig_formula_c))*0.93, (200  + (trig_formula_b + trig_formula_d)*0.93)-blockelevation*elevation, 0.f );
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Wy, trig.vertice_Wx);
            render_flat_quad(mTextureID, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex);
            render_flat_quad(mTextureID, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Ey, trig.vertice_Ex);
            render_flat_quad(mTextureID, trig.vertice_Wy, trig.vertice_Wx, trig.vertice_Wy, trig.vertice_Wx-blockelevation, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ny, trig.vertice_Nx);
            render_flat_quad(mTextureID, trig.vertice_Wy, trig.vertice_Wx, trig.vertice_Wy, trig.vertice_Wx-blockelevation, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Sy, trig.vertice_Sx);
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx-blockelevation, trig.vertice_Ey, trig.vertice_Ex-blockelevation, trig.vertice_Sy, trig.vertice_Sx-blockelevation, trig.vertice_Wy, trig.vertice_Wx-blockelevation);
        }
    }
    mTextureID = 2;
}

void LTexture::renderisometricgrid_userrelation( int visibletilesheight, int visibletileswidth, trigonometry_rendering trig, ObjectArray *chunk, user_perspective user_perspective)
{
    // it's currently chunk_size_eval_increment (1860) / chunk_enum (2) but I'll have to change this eventually
    //Set texture ID
    if (skip == false)
    {
            trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
            trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
            trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
            trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
            glTranslatef(250 + ((trig_formula_a + trig_formula_c))*0.93, (200  + (trig_formula_b + trig_formula_d)*0.93)-blockelevation*elevation, 0.f );
            render_flat_quad(mTextureID, trig.vertice_Ny, trig.vertice_Nx, trig.vertice_Ey, trig.vertice_Ex, trig.vertice_Sy, trig.vertice_Sx, trig.vertice_Wy, trig.vertice_Wx);
    }
}


void LTexture::renderisometricfloor(float Nv1, float Nv2, float Ev1, float Ev2, float Sv1, float Sv2, float Wv1, float Wv2)
{

}

void set_angle(float angle_flag)
{
    if (angle_flag == 1.f)
    {
        i = i+(1/11.25f);
    }
    if (angle_flag == 2.f)
    {
        i = i-(1/11.25f);
    }
}

void LTexture::render_trigonometry_userrelation(controls rendering_controls, ObjectArray *chunk, user_perspective user_perspective, float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig )
{
    int square_chunk_formula = chunk->chunk_size_eval_increment/chunk->chunk_enum;
    elevation = user_perspective.Coord_z-1;
    elevation_increment = 0;
    trig = populate_trig(trig);

    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();
        //Move to rendering point
        glTranslatef( x, y, 0.f );
        int visibletilesheight = user_perspective.Coord_y;
        int zbreadtheight_last, zbreadtheight_lastbon;
        terrain_index = 0;
        for (int zbreadthheight = visibletilesheight; zbreadthheight != chunk->chunk_scope; zbreadthheight++)
            {

                //visibletilesheight++;
                for (int visibletileswidth = user_perspective.angle_reference_x; visibletileswidth != 30.f+user_perspective.angle_reference_x; visibletileswidth++)
                {
                    elevation_increment++;
                    if (elevation_increment >= square_chunk_formula+1)
                    {
                        elevation_increment = 0;
                        visibletilesheight = user_perspective.Coord_y;
                        elevation++;
                    }
            glLoadIdentity();
            if (terrain_index != chunk->chunk_size_eval_increment)
            {
                terrain_index++;
            }
            // Optimization skips rendering if it's trying to render something outside of the window
            if (((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y+rendering_controls.FOV >= 0
                && ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]+user_perspective.angle_reference_x+rendering_controls.FOV >= 0
                && ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]+user_perspective.angle_reference_x-rendering_controls.FOV <= 0
                && ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y-rendering_controls.FOV <= 0 )
                {
                mTextureID = terrain_buffer(chunk, mTextureID);
                if (mTextureID == IDDebugTerrainWall)
                {

                    //renderisometricblock_userrelation( ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y, ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]+user_perspective.Coord_x, trig, chunk, user_perspective);
                    //printf("%d|%d|\n",((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x],((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]);
                    //renderisometricblock_userrelation(visibletilesheight, visibletileswidth, trig, chunk, user_perspective);
                    //printf("Visibletilesheight/width %d |%d : Terrainvariableoordxy %d |%d\n" ,visibletilesheight, visibletileswidth, ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]-15, ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]-15);
                    renderisometricblock_userrelation(
                    ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y,
                    ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]+user_perspective.angle_reference_x-mirage,
                    trig, chunk, user_perspective);
                }
                else
                {
                        //renderisometricblock_userrelation( ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y, visibletileswidth, trig, chunk, user_perspective);
                        //renderisometricgrid_userrelation(visibletilesheight, visibletileswidth, trig, chunk, user_perspective);
                    renderisometricgrid_userrelation(((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_y]+user_perspective.Coord_y, ((Terrain*)chunk->get(terrain_index))->variables[terrain_variable_coord_x]+user_perspective.angle_reference_x-mirage, trig, chunk, user_perspective);
                }
            }
        }
    }
        // this draws the player(s)
        for (int visibletilesheight = user_perspective.Coord_y; visibletilesheight != 31.f; visibletilesheight++)
        {
        // glTranslatef( (x - (trig.vertice_Nx / 2)*visibletilesheight), (y + (trig.vertice_Ny / 4)*visibletilesheight), 0.f );
        for (int visibletileswidth = user_perspective.angle_reference_x; visibletileswidth != 31.f; visibletileswidth++)
            {
                glLoadIdentity();
                trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
                trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
                trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
                trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
                glTranslatef((250 + (trig_formula_a + trig_formula_c)*0.00005), (200  + (trig_formula_b + trig_formula_d)*0.000005) , 0.f );

        if (visibletileswidth == user_perspective.unlabelled_user_perspective_data[1])
            {
                if (visibletilesheight == user_perspective.unlabelled_user_perspective_data[2])
                    {
                        render_flat_sprite(user_debug_sprite_index);
                    }
                }
            }
        }
    }
    // freeTexture();
}

// previous todo: make a function that can render a flat plane with multiple textures at once
// changing the texture in the active buffer is all that I needed to do
void LTexture::render_trigonometry( float angle_flag, GLfloat x, GLfloat y, trigonometry_rendering trig )
{
    trig = populate_trig(trig);
    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();
        //Move to rendering point
        glTranslatef( x, y, 0.f );
        for (int visibletilesheight = 0; visibletilesheight != 30; visibletilesheight++)
        {
        glLoadIdentity();
        // glTranslatef( (x - (trig.vertice_Nx / 2)*visibletilesheight), (y + (trig.vertice_Ny / 4)*visibletilesheight), 0.f );

        for (int visibletileswidth = 0; visibletileswidth != 30; visibletileswidth++)
            {
                glLoadIdentity();
                trig_formula_a = (trig.vertice_W*visibletilesheight*2+((trig.vertice_E-trig.vertice_S)*visibletilesheight));
                trig_formula_b = ((trig.vertice_W-trig.vertice_N)*visibletilesheight*0.535599);
                trig_formula_c = ((trig.vertice_W*visibletileswidth/65.9+((trig.vertice_W-trig.vertice_N)*visibletileswidth)));
                trig_formula_d = ((trig.vertice_E+trig.vertice_S)*visibletileswidth*0.532599);
                glTranslatef(250 + (trig_formula_a + trig_formula_c)*0.93, 200  + (trig_formula_b + trig_formula_d)*0.93, 0.f );
                // +(24*visibletileswidth+(trig.vertice_W-trig.vertice_S)*0.523599)
                // printf("x:%f y:%f\n",trig.vertice_W*visibletilesheight*1.75, trig.vertice_S*visibletilesheight);
            }
        }
    }
    // freeTexture();
}

user_perspective initial_user_perspective(user_perspective* user_perspective, float initial_angle, float initial_visible_portion[array_parameters_for_position])
{
    return *user_perspective;
}

trigonometry_rendering populate_trig(trigonometry_rendering trig_you_modify)
{
            trigonometry_rendering trig;
            //
            trig.vertice_Nx = (40); // (mTextureWidth / 2.f);
            trig.vertice_Ny = 40 * 0.75f; // (mTextureHeight * 0.75f);
            trig.vertice_Ex = (40); // mTextureHeight / 2.f;
            trig.vertice_Ey = (40 * 0.75f); // mTextureWidth;
            trig.vertice_Sx = (40); // (mTextureWidth / 2.f);
            trig.vertice_Sy = (40 * 0.75f); // (mTextureHeight * 0.75f);
            trig.vertice_Wx = (40); //  mTextureWidth
            trig.vertice_Wy = (40 * 0.75f); // mTextureHeight / 2.f;
            //
            trig.angle = trig_you_modify.angle;
            //
            trig.vertice_N = (42 * 0.75f)*glm::sin(i);
            trig.vertice_Ny = ((trig.vertice_Ny+(trig.vertice_N)) * 1.25) / 2;
            trig.vertice_E = -(42 * 0.75f)*glm::cos(i);
            trig.vertice_Ey = ((trig.vertice_Ey+(trig.vertice_E)) * 1.25) / 2;
            trig.vertice_S = -(42 * 0.75f)*glm::sin(i);
            trig.vertice_Sy = ((trig.vertice_Sy+(trig.vertice_S)) * 1.25) / 2;
            trig.vertice_W = (42 * 0.75f)*glm::cos(i);
            trig.vertice_Wy = ((trig.vertice_Wy+(trig.vertice_W)) * 1.25) / 2;
            //
            trig.vertice_N = (42 / 2.f)*glm::cos(i);
            trig.vertice_Nx = (trig.vertice_Nx+(trig.vertice_N)) / 2;
            trig.vertice_E = (42 / 2.f)*glm::sin(i);
            trig.vertice_Ex = (trig.vertice_Ex+(trig.vertice_E)) / 2;
            trig.vertice_S = -(42 / 2.f)*glm::cos(i);
            trig.vertice_Sx = (trig.vertice_Sx+(trig.vertice_S)) / 2;
            trig.vertice_W = -(42 / 2.f)*glm::sin(i);
            trig.vertice_Wx = (trig.vertice_Wx+(trig.vertice_W)) / 2;
        trig.angle = i;
         return trig;
}

GLuint LTexture::getTextureID()
{
    return mTextureID;
}

GLuint LTexture::textureWidth()
{
    return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
    return mTextureHeight;
}

void primitive_screenprint_function(gameworld_constants gameworld_constants_logic, string printing_string){

    // https://www.digitalocean.com/community/tutorials/convert-string-to-char-array-c-plus-plus
    	char arr[printing_string.length() + 1];
	strcpy(arr, printing_string.c_str());
	for (int i = 0; i < printing_string.length(); i++)
    glLoadIdentity();
    for (int x=0; x < printing_string.length(); x++){
            glBindTexture( GL_TEXTURE_2D, (gameworld_constants_logic.termination_of_loaded_alphabet_in_memory-26)+(arr[x]-96) );
            glTranslatef( 8.f, 0.f, 0.f );
            glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(0.f, 0.f);
            glTexCoord2f( 1.f, 0.f ); glVertex2f(8.f, 0.f);
            glTexCoord2f( 1.f, 1.f ); glVertex2f(8.f, 10.f);
            glTexCoord2f( 0.f, 1.f ); glVertex2f(0.f, 10.f);
         glEnd();
    }
}

void dynamic_screenprint_function(float x_pos, float y_pos, gameworld_constants gameworld_constants_logic, string printing_string){

    // https://www.digitalocean.com/community/tutorials/convert-string-to-char-array-c-plus-plus
    	char arr[printing_string.length() + 1];
    glTranslatef( x_pos, y_pos, 0.f );
	strcpy(arr, printing_string.c_str());
	for (int i = 0; i < printing_string.length(); i++)
    glLoadIdentity();
    glTranslatef( x_pos, y_pos, 0.f );
    for (int x=0; x < printing_string.length(); x++){
            glBindTexture( GL_TEXTURE_2D, (gameworld_constants_logic.termination_of_loaded_alphabet_in_memory-26)+(arr[x]-96) );
            glTranslatef( 8.f, 0.f, 0.f );
            glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(0.f, 0.f);
            glTexCoord2f( 1.f, 0.f ); glVertex2f(8.f, 0.f);
            glTexCoord2f( 1.f, 1.f ); glVertex2f(8.f, 10.f);
            glTexCoord2f( 0.f, 1.f ); glVertex2f(0.f, 10.f);
         glEnd();
    }
}

void reset_controls(controls rendering_controls){
    rendering_controls.movement_test = 5.f;
}

void rendering_chunk_setup(gameworld_constants gameworld_constants_logic,
                           ObjectArray* active_chunk,
                           ObjectArray* peripheral_chunk_WN,
                           ObjectArray* peripheral_chunk_N,
                            ObjectArray* peripheral_chunk_NE,
                            ObjectArray* peripheral_chunk_S, ObjectArray* peripheral_chunk_SW,
                             ObjectArray* peripheral_chunk_E, ObjectArray* peripheral_chunk_ES,
                            ObjectArray* peripheral_chunk_W) {

    // One active chunk, eight peripheral chunks NESW
    active_chunk ->peripheral_offset_x = 0;
    active_chunk ->peripheral_offset_y = 0;
    // the north chunk renders 30 tiles to the north -
    //
    peripheral_chunk_WN->peripheral_offset_x = -30;
    peripheral_chunk_WN->peripheral_offset_y = -31;
    //
    peripheral_chunk_N->peripheral_offset_x = -30;
    peripheral_chunk_N->peripheral_offset_y = 0;
    //
    peripheral_chunk_NE->peripheral_offset_x = -30;
    peripheral_chunk_NE->peripheral_offset_y = 31;
    // the east chunk renders 30 tiles to the east -
    peripheral_chunk_S->peripheral_offset_x = 30;
    peripheral_chunk_S->peripheral_offset_y = 0;
    //
    peripheral_chunk_SW->peripheral_offset_x = 30;
    peripheral_chunk_SW->peripheral_offset_y = -31;
    //
    peripheral_chunk_E->peripheral_offset_x = 0;
    peripheral_chunk_E->peripheral_offset_y = 31;
    //
    peripheral_chunk_ES->peripheral_offset_x = 30;
    peripheral_chunk_ES->peripheral_offset_y = 31;
    //
    peripheral_chunk_W->peripheral_offset_x = 0;
    peripheral_chunk_W->peripheral_offset_y = -31;
    //
    active_chunk = describe_chunk("terrain,0,0,0.TXT", active_chunk);
    // is this all I need to do?

    printf("\n\n\nfor example | x:%d y:%d z:%d\n\n\n", ((Terrain*)active_chunk->get(1))->variables[terrain_variable_coord_x],((Terrain*)active_chunk->get(1))->variables[terrain_variable_coord_y],((Terrain*)active_chunk->get(1))->variables[terrain_variable_coord_z]);
    // active_chunk->flatten();
    peripheral_chunk_WN = describe_chunk("terrain,-1,1,0.TXT", peripheral_chunk_WN);
    peripheral_chunk_N = describe_chunk("terrain,0,1,0.TXT", peripheral_chunk_N);
    peripheral_chunk_NE = describe_chunk("terrain,1,1,0.TXT", peripheral_chunk_NE);
    peripheral_chunk_S = describe_chunk("terrain,0,-1,0.TXT", peripheral_chunk_S);
    peripheral_chunk_SW = describe_chunk("terrain,-1,-1,0.TXT", peripheral_chunk_SW);
    peripheral_chunk_E = describe_chunk("terrain,1,0,0.TXT", peripheral_chunk_E);
    peripheral_chunk_ES = describe_chunk("terrain,1,-1,0.TXT", peripheral_chunk_ES);
    peripheral_chunk_W = describe_chunk("terrain,-1,0,0.TXT", peripheral_chunk_W);
    printf("%d\n%d\n",active_chunk->chunk_size_eval_increment, peripheral_chunk_N->chunk_size_eval_increment);
    printf("\n\nenums %d,%d,%d\n\n", active_chunk->chunk_enum, peripheral_chunk_N->chunk_enum, peripheral_chunk_S->chunk_enum);
}

void reloading_upon_memory_leak(const char* filename, gameworld_constants gameworld_constants_logic, SDL_Window* gWindow, ObjectArray* active_chunk) {

          dynamic_screenprint_function(150.f, 120.f, gameworld_constants_logic, "map didnt load correctly arbitrary");
          dynamic_screenprint_function(150.f, 135.f, gameworld_constants_logic, "memory leak issue attempting to reload");
          SDL_GL_SwapWindow( gWindow );
          SDL_Delay(900);
          active_chunk->chunk_enum = 0;
          active_chunk = describe_chunk(filename, active_chunk);
          printf("\n\nenums %d\n\n", active_chunk->chunk_enum);
}

gameworld_constants init_constants(gameworld_constants constants_to_init)
{
    // pointless
    constants_to_init.chunk_zbound = 90;
    constants_to_init.chunk_grid_width = 30;
    constants_to_init.chunk_row_width = 30;
    return constants_to_init;
}
