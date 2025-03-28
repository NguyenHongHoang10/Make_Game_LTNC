#include "GameManager.h"
#include "Constants.h"
#include "Utils.h"
#include "ResourceManager.h"
#include <cmath>

using namespace std;

GameManager::GameManager(SDL_Renderer* renderer) : renderer(renderer), score(0), gameOver(false),
state(MENU), menuTimer(0), groundX(0), shakeTimer(0), shakeOffsetX(0), shakeOffsetY(0) {
    bird = new Bird(renderer);
    for (int i = 0; i < 100; ++i) {
        pipes.push_back(new Pipe(renderer, SCREEN_WIDTH + i * PIPE_SPACING));
    }
    background = ResourceManager::getInstance().getTexture("background");
    ground = ResourceManager::getInstance().getTexture("ground");
    startButton = ResourceManager::getInstance().getTexture("start_button");
    gameOverTexture = ResourceManager::getInstance().getTexture("game_over");
    restartButton = ResourceManager::getInstance().getTexture("restart_button");
    hitSound = ResourceManager::getInstance().getSound("hit");
    dieSound = ResourceManager::getInstance().getSound("die");
    font = ResourceManager::getInstance().getFont("font");

    if (!background || !ground || !startButton || !gameOverTexture
        || !restartButton || !hitSound || !dieSound || !font) {
        logError(cout, "Failed to load resources in GameManager");
        throw runtime_error("Resource loading failed");
    }

    updateScoreTexture();
    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
    updateHighScoreTexture();
}

GameManager::~GameManager() {
    delete bird;
    for (auto& pipe : pipes) delete pipe;
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    // Không cần destroy các tài nguyên khác vì ResourceManager quản lý
}

void GameManager::updateScoreTexture() {
    stringstream ss;
    if (state == GAME_OVER) {
        ss << "Score: " << score;
    } else {
        ss << "Score: " << score;
    }
    string scoreText = ss.str();

    SDL_Color textColor = {255, 165, 0, 255}; // Màu cam
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!surface) {
        logError(cout, "TTF_RenderText_Solid failed: " + string(TTF_GetError()));
        return;
    }

    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!scoreTexture) {
        logError(cout, "SDL_CreateTextureFromSurface failed: " + string(SDL_GetError()));
        surface = TTF_RenderText_Solid(font, "Error", textColor);
        if (surface) {
            scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
}

void GameManager::updateHighScoreTexture() {
    stringstream ss;
    ss << "High Score: " << highScore;
    string highScoreText = ss.str();

    SDL_Color textColor = {0, 0, 255, 255}; // Xanh lam
    SDL_Surface* surface = TTF_RenderText_Solid(font, highScoreText.c_str(), textColor);
    if (!surface) {
        logError(cout, "TTF_RenderText_Solid failed: " + string(TTF_GetError()));
        return;
    }

    if (highScoreTexture) {
        SDL_DestroyTexture(highScoreTexture);
        highScoreTexture = nullptr;
    }
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!highScoreTexture) {
        logError(cout, "SDL_CreateTextureFromSurface failed: " + string(SDL_GetError()));
    }
}

void GameManager::run(bool& restart) {
    restart = false;
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) quit = true;
             handleEvents(e);
        }

        if (state != GAME_OVER) {
            update();
            if (state == PLAYING && checkCollision()) {
                gameOver = true;
                state = GAME_OVER;
                Mix_PlayChannel(-1, hitSound, 0);
                Mix_PlayChannel(-1, dieSound, 0);
                cout << "Game Over! SCORE: " << score << endl;
                shakeTimer = 10;
                updateScoreTexture();
                if (score > highScore) {
                    highScore = score;
                    updateHighScoreTexture();
                    ofstream outFile("highscore.txt");
                    if (outFile.is_open()) {
                        outFile << highScore;
                        outFile.close();
                    }
                }
            }
        }

        render();
        SDL_Delay(1);
    }
}

void GameManager::handleEvents(SDL_Event& e) {
    if (state == MENU) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            int buttonX = SCREEN_WIDTH / 2 - 50;
            int buttonY = SCREEN_HEIGHT / 2 + 50;
            if (mx >= buttonX && mx <= buttonX + 100 && my >= buttonY && my <= buttonY + 100) {
                startGame();
            }
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            startGame();
        }
    } else if (state == PLAYING) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            bird->flap();
        }
    } else if (state == GAME_OVER) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            int buttonX = SCREEN_WIDTH / 2 - 50;
            int buttonY = SCREEN_HEIGHT / 2 + 50;
            if (mx >= buttonX && mx <= buttonX + 100 && my >= buttonY && my <= buttonY + 100) {
                startGame();
            }
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            startGame();
        }
    }
}
