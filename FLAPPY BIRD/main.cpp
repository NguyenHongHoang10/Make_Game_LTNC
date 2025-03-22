#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <ctime>
#include "Constants.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "GameManager.h"

int main(int argc, char* args[])
{
    srand(time(NULL));
    if(initSDL()!=0) return 1;

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        logError(cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr)
    {
        logError(cout, "CreateRenderer");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}
