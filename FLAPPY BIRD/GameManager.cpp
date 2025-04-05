#include "GameManager.h"
#include "Constants.h"
#include "Utils.h"
#include "ResourceManager.h"
#include <cmath>

using namespace std;

GameManager::GameManager(SDL_Renderer* renderer) : renderer(renderer), score(0),
    gameOver(false), state(MENU), menuTimer(0), groundX(0),
        shakeTimer(0), shakeOffsetX(0), shakeOffsetY(0),scrollSpeed(INITIAL_SPEED) {
    bird = new Bird(renderer);
    for (int i = 0; i < 2; ++i) {
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
                scrollSpeed = INITIAL_SPEED;
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
                shakeTimer = 30;
                for(int i=1;i<=shakeTimer;i++)
                {
                    update();
                    render();
                }
                shakeTimer = 0;
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
    }
    else if (state == PLAYING) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            bird->flap();
        }
        else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
        {
            state = PAUSE;
        }
    }
    else if(state == PAUSE)
    {
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
        {
            state = PLAYING;
        }

    }
    else if (state == GAME_OVER) {
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

void GameManager::update() {
    if (state == MENU) {
        menuTimer += 0.05;
        bird->setPosition(SCREEN_WIDTH / 4, static_cast<int>(SCREEN_HEIGHT / 2 + sin(menuTimer) * 30));
    } else if (state == PLAYING) {
        bird->update();
        int maxPipeX = pipes[0]->getTopRect(0, 0).x;
        for (const auto& pipe : pipes) {
            int pipeX = pipe->getTopRect(0, 0).x;
            if (pipeX > maxPipeX) {
                maxPipeX = pipeX;
            }
        }
        for (auto& pipe : pipes) {
            pipe->update(scrollSpeed);
            if (pipe->getTopRect(0, 0).x + PIPE_WIDTH < bird->getRect().x && !pipe->hasPassed()) {
                pipe->markPassed();
                score++;
                cout << "Diem: " << score << endl;
                if (score % Kpipe == 0) {
                    scrollSpeed += INCREASE;
                    cout << "Scroll speed increased to: " << scrollSpeed << endl;
                    shakeTimer = 15;
                }

                updateScoreTexture();
            }
            if (pipe->getTopRect(0, 0).x + PIPE_WIDTH < 0) {
                pipe->reset(maxPipeX + PIPE_SPACING);
                cout << "Pipe reset at x=" << pipe->getTopRect(0, 0).x << endl;
                maxPipeX = pipe->getTopRect(0, 0).x;
            }
        }

        groundX -= scrollSpeed;
        if (groundX < -SCREEN_WIDTH) groundX += SCREEN_WIDTH;

    }
    if (shakeTimer > 0) {
        shakeOffsetX = (rand() % 10) - 5;
        shakeOffsetY = (rand() % 10) - 5;
        shakeTimer--;
    } else {
        shakeOffsetX = 0;
        shakeOffsetY = 0;
    }
}

void GameManager::render() {
    clearRenderer(renderer);
    SDL_Rect backgroundRect = {shakeOffsetX, shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, background, NULL, &backgroundRect);

    for (auto& pipe : pipes) pipe->render(shakeOffsetX, shakeOffsetY);
    bird->render(shakeOffsetX, shakeOffsetY);
    SDL_Rect groundRect = {static_cast<int>(groundX) + shakeOffsetX, SCREEN_HEIGHT - GROUND_HEIGHT + shakeOffsetY, SCREEN_WIDTH, GROUND_HEIGHT};
    SDL_Rect groundRect2 = {static_cast<int>(groundX + SCREEN_WIDTH) + shakeOffsetX, SCREEN_HEIGHT - GROUND_HEIGHT + shakeOffsetY, SCREEN_WIDTH, GROUND_HEIGHT};
    SDL_RenderCopy(renderer, ground, NULL, &groundRect);
    SDL_RenderCopy(renderer, ground, NULL, &groundRect2);
    if (state == MENU) {
        SDL_Rect buttonRect = {SCREEN_WIDTH / 2 - 50 + shakeOffsetX, SCREEN_HEIGHT / 2 + 50 + shakeOffsetY, 100, 100};
        SDL_RenderCopy(renderer, startButton, NULL, &buttonRect);
    }
    else if (state == GAME_OVER) {
        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150 + shakeOffsetX, SCREEN_HEIGHT / 2 - 150 + shakeOffsetY, 300, 100};
        SDL_Rect restartRect = {SCREEN_WIDTH / 2 - 50 + shakeOffsetX, SCREEN_HEIGHT / 2 + 50 + shakeOffsetY, 100, 100};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_RenderCopy(renderer, restartButton, NULL, &restartRect);
    }
    else if (state == PAUSE) {
    SDL_Color white = {255, 215, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Paused", white);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25, 100, 50};
    SDL_RenderCopy(renderer, tempTexture, NULL, &pauseRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tempTexture);
    }


    if (highScoreTexture && state == MENU) {
        int w, h;
        SDL_QueryTexture(highScoreTexture, NULL, NULL, &w, &h);
        SDL_Rect highScoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - 50 + shakeOffsetY, w, h};
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreDst);
    }

    if (scoreTexture && state != MENU) {
        int w, h;
        SDL_QueryTexture(scoreTexture, NULL, NULL, &w, &h);
        SDL_Rect scoreDst;
        if (state == GAME_OVER) {
            scoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - 40 + shakeOffsetY, w, h};
        } else if (state == PLAYING) {
            scoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCORE_POS_Y + shakeOffsetY, w, h};
        }
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreDst);
    }

    if (highScoreTexture && state == GAME_OVER) {
        int w, h;
        if (SDL_QueryTexture(highScoreTexture, NULL, NULL, &w, &h) == 0) {
            SDL_Rect highScoreDst = {SCREEN_WIDTH / 2 - w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 + 10 + shakeOffsetY, w, h};
            SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreDst);
        } else {
            cout << "SDL_QueryTexture failed for highScoreTexture: " << SDL_GetError() << endl;
        }
    }
    SDL_RenderPresent(renderer);
}

bool GameManager::checkCollision() {
    SDL_Rect birdRect = bird->getRect();
    for (auto& pipe : pipes) {
        SDL_Rect topRect = pipe->getTopRect(shakeOffsetX, shakeOffsetY);
        SDL_Rect bottomRect = pipe->getBottomRect(shakeOffsetX, shakeOffsetY);
        if (SDL_HasIntersection(&birdRect, &topRect)) {
            cout << "Collision with top pipe: birdRect(x=" << birdRect.x << ", y=" << birdRect.y
                      << ", w=" << birdRect.w << ", h=" << birdRect.h
                      << "), topRect(x=" << topRect.x << ", y=" << topRect.y
                      << ", w=" << topRect.w << ", h=" << topRect.h << ")" << endl;
            return true;
        }
        if (SDL_HasIntersection(&birdRect, &bottomRect)) {
            cout << "Collision with bottom pipe: birdRect(x=" << birdRect.x << ", y=" << birdRect.y
                      << ", w=" << birdRect.w << ", h=" << birdRect.h
                      << "), bottomRect(x=" << bottomRect.x << ", y=" << bottomRect.y
                      << ", w=" << bottomRect.w << ", h=" << bottomRect.h << ")" << endl;
            return true;
        }
    }

    if (birdRect.y + BIRD_HEIGHT >= SCREEN_HEIGHT - GROUND_HEIGHT) {
        cout << "Collision with ground: birdRect(y=" << birdRect.y << ")" << endl;
        return true;
    }
    return false;
}

void GameManager::startGame() {
    bird->setPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
    bird->flap();
    for (size_t i = 0; i < pipes.size(); ++i) {
        pipes[i]->reset(SCREEN_WIDTH + i * PIPE_SPACING);
    }
    score = 0;
    updateScoreTexture();
    gameOver = false;
    state = PLAYING;
}
