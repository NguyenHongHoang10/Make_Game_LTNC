#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "Bird.h"
#include "Pipe.h"
#include "GameState.h"

using namespace std;

class GameManager {
private:
    SDL_Renderer* renderer;
    Bird* bird;
    vector<Pipe*> pipes;
    SDL_Texture* background;
    SDL_Texture* ground;
    SDL_Texture* startButton;
    SDL_Texture* gameOverTexture;
    SDL_Texture* restartButton;
    Mix_Chunk* hitSound;
    Mix_Chunk* dieSound;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    int score;
    SDL_Texture* highScoreTexture;
    int highScore = 0;
    bool gameOver;
    GameState state;
    float menuTimer;
    float groundX;
    int shakeTimer;
    int shakeOffsetX;
    int shakeOffsetY;
    float scrollSpeed;


    void handleEvents(SDL_Event& e);
    void update();
    void render();
    bool checkCollision();
    void startGame();
    void updateScoreTexture();
    void updateHighScoreTexture();

public:
    GameManager(SDL_Renderer* renderer);
    ~GameManager();
    void run(bool& restart);
};

#endif
