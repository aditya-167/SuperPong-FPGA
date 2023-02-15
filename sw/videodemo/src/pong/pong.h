#ifndef PONG_H
#define PONG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#ifdef WIN32
	#include <SDL2/SDL.h>
#endif

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
bool initialize(void);
void update(float);
void shutdown(void);
void scoreUpdate(int, int);

// Ball functions
void render_ball(const BallStruct *);
void update_ball(BallStruct *, float);

ball_struct init_ball(int size);

#endif