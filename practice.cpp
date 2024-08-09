#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH { 640 };
const int SCREEN_HEIGHT { 480 };

SDL_Window* gWindow;
SDL_Surface *gScreenSurface;

bool init();
bool loadMedia();
void close();

bool init() {
    bool success { true };

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Initailization error. SDL_Error : " << SDL_GetError() << std::endl;
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("practice window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            std::cerr << "Failed to create window. SDL_Error : " << SDL_GetError() << std::endl;
            success = false;
        }
        else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            if (gScreenSurface == NULL) {
                std::cerr << "Failed to get the window surface. SDL_Error : " << SDL_GetError() << std::endl;
                success = false;
            }
            else {
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cerr << "SDL_Image could not initialize. SDL_image Error : " << IMG_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }
}

bool loadMedia(std::string path) {
    SDL_Surface* optimizedSurface { NULL };
    SDL_Surface* loadedMedia = IMG_Load(path.c_str());
    if (loadedMedia == NULL) {
        std::cerr << "Failed to load the media in the path " << path << ". SDL_image Error : " << IMG_GetError() << std::endl;
    }
    else {
        optimizedSurface = SDL_ConvertSurface(loadedMedia, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            std::cerr << "Failed to optimize the media. SDL_Error : " <<  SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedMedia);
        loadedMedia = NULL;
    }
    return optimizedSurface;
}