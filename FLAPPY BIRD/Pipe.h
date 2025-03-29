#ifndef PIPE_H
#define PIPE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Constants.h"
#include "Utils.h"

class Pipe {
private:
    SDL_Renderer* renderer;
    SDL_Texture* endTexture;
    SDL_Texture* bodyTexture;
    Mix_Chunk* swooshSound;
    Mix_Chunk* pointSound;
    int x;
    int topHeight;
    int bottomY;
    bool passed;
    int endHeight;
    int bodyHeight;

public:
    Pipe(SDL_Renderer* renderer, int startX);
    ~Pipe();
    void update(float speed);
    void render(int shakeOffsetX, int shakeOffsetY);
    void reset(int startX);
    SDL_Rect getTopRect(int shakeOffsetX, int shakeOffsetY);
    SDL_Rect getBottomRect(int shakeOffsetX, int shakeOffsetY);
    bool hasPassed();
    void markPassed();
};

#endif
