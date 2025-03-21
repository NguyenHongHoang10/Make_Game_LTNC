#include "Utils.h"

void logError(ostream& os, const string& msg)
{
    os << msg << "Error: " << SDL_GetError() << endl;
}
int initSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logError(cout, "SDL_Init");
        return 1;
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        logError(cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        logError(cout, "Mix_OpenAudio")'
        SDL_Quit();
        return 1;
    }
    if(TTF_Init() == -1)
    {
        logError(cout, "TTF_Init");
        return 1;
    }
    return 0;
}

SDL_Texture* loadTexture(string& path, SDL_Renderer* renderer)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.cstr());
    if(!texture)
    {
        logError(cout, "Load " + path);
    }
    return texture;
}
void clearRenderer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Mau nen xanh nhat
    SDL_RenderClear(renderer);
}
