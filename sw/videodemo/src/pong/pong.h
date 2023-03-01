#ifndef PONG_H
#define PONG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../utility/engine.h"
#include "sys/time.h"
#include "../graphics/graphics.h"
#include "xparameters.h"
#include "xtmrctr.h"
#include "xil_printf.h"

#define TMRCTR_DEVICE_ID	XPAR_TMRCTR_0_DEVICE_ID
#define TIMER_COUNTER_0	 0

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
#define BALLVEL 7
#define BALLSIZE 10
#define PLAYERPAD_WIDTH 10
#define PLAYERPAD_HEIGHT 100
#define PLAYERPAD_MARGIN 1
#define PLAYERPAD_VELOCITY 8

typedef enum GAME_STATE {
	RUNNING, PAUSED, STOPPED
} GAME_STATE;

// Ball structure
typedef struct ball_struct {
	int x;
	int y;
	int pX;
	int pY;
	int xVel;
	int yVel;
	int size;
} ball_struct;

// PlayerPad structure
typedef struct player_pad {
	int xPos;
	int yPos;
	int pX;
	int pY;
	int w;
	int h;
	int score;
} player_pad;

// Game context structure
typedef struct game_context {

	player_pad* pad1;
	player_pad* pad2;
	ball_struct* ball;
	volatile unsigned int* keys;
	enum GAME_STATE state;

} game_context;

// Game functions
bool initialize(game_context* game, unsigned int* input_address);
void update(int t_elapse, game_context* game, graphics_context* gc);
void shutdown(game_context* game);
void scoreUpdate(int, int);
int setup_stopwatch(XTmrCtr* TimerCounter);
int start_stopwatch(XTmrCtr* TmrCtrInstancePtr);
int end_stopwatch(XTmrCtr* TmrCtrInstancePtr);

// Ball functions
void render_ball(game_context* game, graphics_context* gc);
void update_ball(game_context *game, GAME_STATE state, int t_elapse);
void check_interesections(game_context* g, graphics_context* gc);
ball_struct* init_ball(int size);

// Pad function
void update_CPU_pad(player_pad* pad2, ball_struct* ball, int t_elapse);
void update_player_pad(player_pad* pad1, player_pad* pad2, ball_struct* ball,
		int t_elapse, int keys);
void render_pads(game_context* game, graphics_context* gc);
player_pad* init_player_pad(void);

// Debugging
void print_state(game_context *g);

#endif
