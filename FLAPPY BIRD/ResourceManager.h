#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <map>

class ResourceManager {
public:
    static ResourceManager& getInstance();
    void loadTextureResource(const std::string& name, const std::string& path, SDL_Renderer* renderer);
    SDL_Texture* getTexture(const std::string& name);
    void loadSound(const std::string& name, const std::string& path);
    Mix_Chunk* getSound(const std::string& name);
    void loadFont(const std::string& name, const std::string& path, int size);
    TTF_Font* getFont(const std::string& name);
    void cleanup();

private:
    ResourceManager() {}
    ~ResourceManager();
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, Mix_Chunk*> sounds;
    std::map<std::string, TTF_Font*> fonts;
};

#endif
