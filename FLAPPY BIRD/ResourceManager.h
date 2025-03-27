#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <map>
using namespace std;
class ResourceManager {
public:
    static ResourceManager& getInstance();
    void loadTextureResource(const string& name, const string& path, SDL_Renderer* renderer);
    SDL_Texture* getTexture(const string& name);
    void loadSound(const string& name, const string& path);
    Mix_Chunk* getSound(const string& name);
    void loadFont(const string& name, const string& path, int size);
    TTF_Font* getFont(const string& name);
    void cleanup();

private:
    ResourceManager() {}
    ~ResourceManager();
    map<string, SDL_Texture*> textures;
    map<string, Mix_Chunk*> sounds;
    map<string, TTF_Font*> fonts;
};

#endif
