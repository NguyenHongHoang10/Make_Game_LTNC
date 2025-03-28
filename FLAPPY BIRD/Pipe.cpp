#include "Pipe.h"
#include "Constants.h"
#include "Utils.h"
#include "ResourceManager.h"
#include <cstdlib>

using namespace std;

Pipe::Pipe(SDL_Renderer* renderer, int startX) : renderer(renderer), x(startX), passed(false) {
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN;
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER;
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;

    endTexture = ResourceManager::getInstance().getTexture("pipe_end");
    bodyTexture = ResourceManager::getInstance().getTexture("pipe_body");
    swooshSound = ResourceManager::getInstance().getSound("swoosh");
    pointSound = ResourceManager::getInstance().getSound("point");

    if (!endTexture || !bodyTexture || !swooshSound || !pointSound) {
        cerr << "Failed to load pipe resources" << endl;
        throw runtime_error("Pipe resource loading failed");
    }

    SDL_QueryTexture(endTexture, NULL, NULL, NULL, &endHeight);
    SDL_QueryTexture(bodyTexture, NULL, NULL, NULL, &bodyHeight);
}

Pipe::~Pipe() {
    // Không cần destroy texture hay sound vì ResourceManager quản lý
}

void Pipe::update() {
    x -= SCROLL_SPEED;
}

void Pipe::render(int shakeOffsetX, int shakeOffsetY) {
    if (topHeight > 0) {
        int capHeight = min(endHeight, topHeight);
        SDL_Rect topEndDst = {x + shakeOffsetX, topHeight - capHeight + shakeOffsetY, PIPE_WIDTH, capHeight};
        SDL_RenderCopy(renderer, endTexture, NULL, &topEndDst);

        int remainingHeight = topHeight - capHeight;
        if (remainingHeight > 0) {
            for (int y = 0; y < remainingHeight; y += bodyHeight) {
                int segmentHeight = min(bodyHeight, remainingHeight - y);
                SDL_Rect topBodyDst = {x + shakeOffsetX, y + shakeOffsetY, PIPE_WIDTH, segmentHeight};
                SDL_RenderCopy(renderer, bodyTexture, NULL, &topBodyDst);
            }
        }
    }

    int bottomHeight = SCREEN_HEIGHT - GROUND_HEIGHT - bottomY;
    if (bottomHeight > 0) {
        int capHeight = min(endHeight, bottomHeight);
        SDL_Rect bottomEndDst = {x + shakeOffsetX, bottomY + shakeOffsetY, PIPE_WIDTH, capHeight};
        SDL_RenderCopy(renderer, endTexture, NULL, &bottomEndDst);

        int remainingHeight = bottomHeight - capHeight;
        if (remainingHeight > 0) {
            int targetY = SCREEN_HEIGHT - GROUND_HEIGHT;
            int startY = bottomY + capHeight;
            int y = startY;
            while (y < targetY) {
                int segmentHeight = min(bodyHeight, targetY - y);
                if (segmentHeight <= 0) break;
                SDL_Rect bottomBodyDst = {x + shakeOffsetX, y + shakeOffsetY, PIPE_WIDTH, segmentHeight};
                SDL_RenderCopy(renderer, bodyTexture, NULL, &bottomBodyDst);
                y += segmentHeight;
            }
        }
    }
}

void Pipe::reset(int startX) {
    x = startX;
    passed = false;
    int gapSize = rand() % (GAP_MAX - GAP_MIN + 1) + GAP_MIN;
    int gapCenter = rand() % (MAX_GAP_CENTER - MIN_GAP_CENTER + 1) + MIN_GAP_CENTER;
    if (gapCenter - gapSize / 2 < endHeight) {
        gapCenter = endHeight + gapSize / 2;
    }
    if (gapCenter + gapSize / 2 > SCREEN_HEIGHT - GROUND_HEIGHT - endHeight) {
        gapCenter = SCREEN_HEIGHT - GROUND_HEIGHT - endHeight - gapSize / 2;
    }
    topHeight = gapCenter - gapSize / 2;
    bottomY = gapCenter + gapSize / 2;
}

SDL_Rect Pipe::getTopRect(int shakeOffsetX, int shakeOffsetY) {
    int collisionWidth = max(0, PIPE_WIDTH - 2 * COLLISION_OFFSET);
    return {x + shakeOffsetX + COLLISION_OFFSET, shakeOffsetY, collisionWidth, topHeight - 18};
}

SDL_Rect Pipe::getBottomRect(int shakeOffsetX, int shakeOffsetY) {
    int collisionWidth = max(0, PIPE_WIDTH - 2 * COLLISION_OFFSET);
    return {x + shakeOffsetX + COLLISION_OFFSET, bottomY + shakeOffsetY, collisionWidth, SCREEN_HEIGHT - GROUND_HEIGHT - bottomY};
}

bool Pipe::hasPassed() {
    return passed;
}

void Pipe::markPassed() {
    if (!passed) {
        passed = true;
        Mix_PlayChannel(-1, swooshSound, 0);
        Mix_PlayChannel(-1, pointSound, 0);
    }
}
