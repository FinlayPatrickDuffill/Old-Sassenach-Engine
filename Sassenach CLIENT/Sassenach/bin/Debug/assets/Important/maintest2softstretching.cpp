#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <init.h>

/* Initialization:
"void close();" frees media and shuts down SDL,
"bool init()" initializes SDL video
"bool loadMedia();" loads media
*/

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video
    if (!init()) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Initialize SDL_image for PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    // 3. Create Window and Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    //Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}

    // 5. Main Loop
    bool quit = false;
    SDL_Event e;
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Apply the image stretched
				SDL_Rect stretchRect;
				stretchRect.x = 30;
				stretchRect.y = 30;
				stretchRect.w = 40;
				stretchRect.h = 20;
				SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );
				stretchRect.x = 30;
				stretchRect.y = 30;
				stretchRect.w = 40;
				stretchRect.h = 20;
				SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );

				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}

    // 6. Cleanup
    void close();

    return 0;
}
