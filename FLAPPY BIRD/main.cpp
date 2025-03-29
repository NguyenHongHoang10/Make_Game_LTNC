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


using namespace std;
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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    /// Tải tài nguyên
    ResourceManager& resourceManager = ResourceManager::getInstance();
    resourceManager.loadTextureResource("background", "Assets/background.png", renderer);
    resourceManager.loadTextureResource("ground", "Assets/ground.png", renderer);
    resourceManager.loadTextureResource("bird", "Assets/bird.png", renderer);
    resourceManager.loadTextureResource("pipe_end", "Assets/pipe_end.png", renderer);
    resourceManager.loadTextureResource("pipe_body", "Assets/pipe_body.png", renderer);
    resourceManager.loadTextureResource("start_button", "Assets/start_button.png", renderer);

    resourceManager.loadTextureResource("game_over", "Assets/game_over.png", renderer);
    resourceManager.loadTextureResource("restart_button", "Assets/restart_button.png", renderer);
    resourceManager.loadSound("flap", "Assets/flap_sound.mp3");
    resourceManager.loadSound("hit", "Assets/bird_hit_sound.mp3");
    resourceManager.loadSound("die", "Assets/die_sound.mp3");
    resourceManager.loadSound("swoosh", "Assets/swoosh_sound.mp3");
    resourceManager.loadSound("point", "Assets/point_sound.mp3");
    resourceManager.loadFont("font", FONT_PATH, FONT_SIZE);

    try
    {
        GameManager game(renderer);
        bool restart = true;
        while(restart)
        {
            game.run(restart);
        }

    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    resourceManager.cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
