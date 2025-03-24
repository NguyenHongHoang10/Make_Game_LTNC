#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

using namespace std;
/// Thông số Game
const string WINDOW_TITLE = "Flappy Bird";
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 64;
const int BIRD_HEIGHT = 64;
const int SCROLL_SPEED = 2;
const int PIPE_WIDTH = 52;
const int PIPE_SPACING = 250;
const int GAP_MIN = 135;
const int GAP_MAX = 135;
const int COLLISION_OFFSET = 0;
const double GRAVITY = 0.4f;
const double FLAP_FORCE = -7.0f;
const int MIN_GAP_CENTER = 120;
const int MAX_GAP_CENTER = 350;
const int GROUND_HEIGHT = 100;

/// Thông số hiển thị điểm số
const string FONT_PATH = "font.ttf";
const int FONT_SIZE = 24;
const int SCORE_POS_X = 10;
const int SCORE_POS_Y = 10;
const int GAME_OVER_SCORE_POS_X = SCREEN_WIDTH/2 - 50;
const int GAME_OVER_SCORE_POS_Y = SCREEN_HEIGHT/2;




#endif // CONSTANTS_H
