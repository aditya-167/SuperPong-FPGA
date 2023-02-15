#ifndef PONG_H
#define PONG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define SCREEN_HEIGHT 480;
#define SCREEN_WIDTH 640;
#define BALLVEL 130;
#define BALLSIZE 10;
#define PLAYERPAD_WIDTH 22;
#define PLAYERPAD_HEIGHT 75;
#define PLAYERPAD_MARGIN 10;
#define PLAYERPAD_VELOCITY 180.0f;

// Ball structure
typedef struct ball_struct{
    float x;
    float y;
    float xVel;
    float yVel;
    int size;
} ball_struct;

// PlayerPad structure
typedef struct player_pad{
    float yPos;
    int score;
} player_pad;

// Game functions
bool initialize(ball_struct *ball, player_pad *pad1, player_pad *pad2);
void update(float);
void shutdown(void);
void scoreUpdate(int, int);

// Ball functions
void render_ball(const ball_struct *);
void update_ball(ball_struct *, float);

ball_struct init_ball(int size);

#endif
