#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"
#include <filesystem>
#include "LMapLoading.h"
#include <SDL.h>
#include <SDL_events.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

chunk_storage *load_chunk_from_file(const char *name_of_file) {


            cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
            FILE* fp = std::fopen(name_of_file, "w+");
    if (!fp)
    {
        std::perror("File opening failed");
    }
            string line;
            size_t len, pos;
            int size_eval_increment;
            bool isFound;
            int file_sizing = filesize(name_of_file);

            chunk_storage * chunk_to_store;

            int c; // Note: int, not char, required to handle EOF
            while ((c = std::fgetc(fp)) != EOF) {
                        if (c == '#')
            {
                    size_eval_increment++;
                    printf("Turf: # %d \n", size_eval_increment);
            }
            else
            {
                cout << "A line in the config file contains bad syntax..." << endl;
            }
            };

            chunk_to_store->chunk_size = size_eval_increment * size_eval_increment;

            //chunk_to_store = malloc(sizeof(chunk_storage));

            return chunk_to_store;
};

ObjectArray* describe_chunk(const char *name_of_file, ObjectArray *chunk) {

    int squareroot = 0;
        cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
        FILE* fp = std::fopen(name_of_file, "r");
        if (!fp)
            {
                std::perror("File opening failed");
            }
        else
        {
            printf("File opened\n");
        }
        string line;
        size_t len, pos = 0;
        int size_eval_increment = 0;
        bool isFound;
        int file_sizing = filesize(name_of_file);

            int c; // Note: int, not char, required to handle EOF
            while ((c = std::fgetc(fp)) != EOF) {
                        if (c == '#')
            {
                //DEBUG_TERRAIN
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN"));
            }
                        if (c == '/')
            {
                    chunk->chunk_enum++;
            }
                        if (c == 'A')
            {
                    size_eval_increment++;

                    chunk->add(new Terrain("DEBUG_TERRAIN_INVERT"));
            }
                        if (c == 'O')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN_WALL"));
            }
                        if (c == 'y')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("AIR"));
            }
            };
            squareroot = sqrt(size_eval_increment/chunk->chunk_enum);
            // printf("Size eval increment %d\n",size_eval_increment);
            // printf("chunk_enum %d\n",chunk->chunk_enum);
            // printf("chunk_scope %d\n",chunk->chunk_scope);
            chunk->chunk_size_eval_increment = size_eval_increment;
            chunk->chunk_scope = squareroot * chunk->chunk_enum;
            // printf("Chunk size eval increment/2 %d\n",chunk->chunk_size_eval_increment);
            chunk->flatten();

            int coord_x, coord_y, coord_z = -1;

            for (int x = 0; x != chunk->chunk_size_eval_increment; x++)
            {

                 if (coord_x == 30)
                    {
                        coord_y++;
                        coord_x = 0;
                    }
                if (coord_y == 31)
                    {
                        coord_z++;
                        coord_y = 0;
                        coord_x = 0;
                    }
                coord_x++;
                ((Terrain*)chunk->get(x))->variables[terrain_variable_coord_x] = coord_x+chunk->peripheral_offset_x;
                ((Terrain*)chunk->get(x))->variables[terrain_variable_coord_y] = coord_y+chunk->peripheral_offset_y;
                ((Terrain*)chunk->get(x))->variables[terrain_variable_coord_z] = coord_z;
            }
    for (int gh = 0; gh!= 100; gh++)
    // printf("Chunk breadth: %d |x:%dy:%dz:%d\n",chunk->chunk_breadth,((Terrain*)chunk->get(gh))->variables[terrain_variable_coord_x],((Terrain*)chunk->get(gh))->variables[terrain_variable_coord_y],((Terrain*)chunk->get(gh))->variables[terrain_variable_coord_z]);
            return chunk;
};

void describe_peripheral_chunk(const char *name_of_file, ObjectArray *chunk) {

    int squareroot;
        cout << "Loading a chunk '(' "<< name_of_file <<" ')..." << endl;
        FILE* fp = std::fopen(name_of_file, "r");
        if (!fp)
            {
                std::perror("File opening failed");
            }
        else
        {
            printf("File opened\n");
        }
        string line;
        size_t len, pos;
        int size_eval_increment;
        bool isFound;
        int file_sizing = filesize(name_of_file);
            int c; // Note: int, not char, required to handle EOF
            while ((c = std::fgetc(fp)) != EOF) {
                        if (c == '#')
            {
                //DEBUG_TERRAIN
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN"));
            }
                        if (c == '/')
            {
                    chunk->chunk_enum++;
            }
                        if (c == 'A')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN_INVERT"));
            }
                        if (c == 'O')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("DEBUG_TERRAIN_WALL"));
            }
                        if (c == 'y')
            {
                    size_eval_increment++;
                    chunk->add(new Terrain("AIR"));
            }
            };
            squareroot = sqrt(size_eval_increment/chunk->chunk_enum);
            printf("Size eval increment %d\n",size_eval_increment);
            printf("chunk_enum %d\n",chunk->chunk_enum);
            printf("chunk_scope %d\n",chunk->chunk_scope);
            chunk->chunk_size_eval_increment = size_eval_increment;
            chunk->chunk_scope = squareroot * chunk->chunk_enum;
            printf("Chunk size eval increment/2 %d\n",chunk->chunk_size_eval_increment);
};
