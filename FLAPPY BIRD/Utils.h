#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

int initSDL();
SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer);
void logError(ostream& os, const string& msg);
void clearRenderer(SDL_Renderer* renderer);

#endif // UTILS_H
