#ifndef PONG_H
#define PONG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../utility/engine.h"
#include "sys/time.h"
#ifndef WIN32
	#include "../graphics/graphics.h"
#endif

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define BALLVEL 130
#define BALLSIZE 10
#define PLAYERPAD_WIDTH 22
#define PLAYERPAD_HEIGHT 75
#define PLAYERPAD_MARGIN 10
#define PLAYERPAD_VELOCITY 180.0f

typedef enum GAME_STATE{
	RUNNING, PAUSED, STOPPED
} GAME_STATE;

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
    float xPos;
	float yPos;
    int score;
} player_pad;

// Game context structure
typedef struct game_context {

	player_pad* pad1;
	player_pad* pad2;
	ball_struct* ball;
	enum GAME_STATE state;
} game_context;


// Game functions
bool initialize(game_context* game);
void update(float t_elapse, game_context* game, graphics_context* gc);
void shutdown(game_context* game);
void scoreUpdate(int, int);

// Ball functions
void render_ball(game_context* game, graphics_context* gc);
void update_ball(game_context *game, GAME_STATE state, float t_elapse);
ball_struct* init_ball(int size);

// Pad function
void update_CPU_pad(player_pad* pad2, ball_struct* ball, float t_elapse);
void update_player_pad(player_pad* pad1, player_pad* pad2, ball_struct* ball, float t_elapse);
void render_pads(game_context* game, graphics_context* gc);
player_pad* init_player_pad(void);

#endif
