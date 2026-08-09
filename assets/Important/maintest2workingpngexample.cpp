#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
    SDL_Window* window = SDL_CreateWindow("SDL2_image 2.6.1 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 4. Load Image using IMG_LoadTexture (Modern SDL2_image)
    // IMG_LoadTexture works directly with renderer to create a GPU texture
    SDL_Texture* texture = IMG_LoadTexture(renderer, "image.png");
    if (!texture) {
        std::cerr << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        // Handle error
    }

    // 5. Main Loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Render texture
        if (texture) {
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // 6. Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
