#include "ResourceManager.h"
#include "Utils.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadTextureResource(const std::string& name, const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* texture = loadTexture(path, renderer);
    if (texture) {
        textures[name] = texture;
    }
}

SDL_Texture* ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    return (it != textures.end()) ? it->second : nullptr;
}

void ResourceManager::loadSound(const std::string& name, const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound) {
        sounds[name] = sound;
    } else {
        logError(std::cout, "Failed to load sound: " + path);
    }
}

Mix_Chunk* ResourceManager::getSound(const std::string& name) {
    auto it = sounds.find(name);
    return (it != sounds.end()) ? it->second : nullptr;
}

void ResourceManager::loadFont(const std::string& name, const std::string& path, int size) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (font) {
        fonts[name] = font;
    } else {
        logError(std::cout, "Failed to load font: " + path);
    }
}

TTF_Font* ResourceManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    return (it != fonts.end()) ? it->second : nullptr;
}

void ResourceManager::cleanup() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
    for (auto& pair : sounds) {
        Mix_FreeChunk(pair.second);
    }
    sounds.clear();
    for (auto& pair : fonts) {
        TTF_CloseFont(pair.second);
    }
    fonts.clear();
}

ResourceManager::~ResourceManager() {
    cleanup();
}
