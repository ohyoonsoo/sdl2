#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurface {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};


bool init();
bool loadMedia();
void close();

SDL_Window* gWindow { NULL };
SDL_Surface* gScreenSurface { NULL };
SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface { NULL };



bool init() {
    bool success { true };

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        success = false;
        std::cerr << "Failed to initialize. SDL_Error : " << SDL_GetError() << std::endl;
    }
    else {
        gWindow = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            success = false;
            std::cerr << "Failed to create the window. SDL_Error : " << SDL_GetError() << std::endl;
        }
        else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            if (gScreenSurface == NULL) {
                success = false;
                std::cerr << "Failed to get the window surface. SDL_Error : " << SDL_GetError() << std::endl;
            }
        }
    }
    return success;
}

SDL_Surface* loadSurface(const std::string& path) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Surface* optimizedSurface { NULL };
    if (loadedSurface == NULL) {
        std::cerr << "Failed to load the bmp file " << path << ". SDL_Error : " << SDL_GetError() << std::endl;
    }
    else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            std::cerr << "Failed to optimize the loaded surfaece with bmp file. SDL_Error : " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

bool loadMedia() {
    bool success { true };

    gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("./media/press.bmp");
    if(gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        std::cerr << "Failed to load default image" << std::endl;
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("./media/up.bmp");
    if(gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL) {
        std::cerr << "Failed to load up image" << std::endl;
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("./media/down.bmp");
    if(gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL) {
        std::cerr << "Failed to load down image" << std::endl;
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("./media/left.bmp");
    if(gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL) {
        std::cerr << "Failed to load left image" << std::endl;
        success = false;
    }

    gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("./media/right.bmp");
    if(gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        std::cerr << "Failed to load right image" << std::endl;
        success = false;
    }

    return success;
}


void close() {
    for (auto surface : gKeyPressSurface) {
        std::cout << "debug" << std::endl; 
        SDL_FreeSurface(surface);
        surface = NULL;
    }
    gCurrentSurface = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gScreenSurface = NULL;
    SDL_Quit();
}

int main() {
    bool success = true;
    success = init();
    if (success == false) {
        std::cerr << "Error occurred during the initialization." << std::endl;
    }
    success = loadMedia();
    if (success == false) {
        std::cerr << "Error occured during loading the bmp files." << std::endl;
    }

    gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

    SDL_Event e;
    bool quit { false };
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN:
                        gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
                        break;
                    case SDLK_LEFT:
                        gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
                        break;
                    case SDLK_RIGHT:
                        gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                        break;
                    default:
                        gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                }

                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();

    return 0;
}