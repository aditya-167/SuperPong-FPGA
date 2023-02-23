#include "pong.h"

// Initialise the game
bool initialize(game_context* game) {

	ball_struct* ball = init_ball(BALLSIZE);
	player_pad* pad1 = init_player_pad();
	player_pad* pad2 = init_player_pad();

	pad1->xPos = 50;
	pad2->xPos = SCREEN_WIDTH - 50;

	game->ball = ball;
	game->pad1 = pad1;
	game->pad2 = pad2;
	game->state = RUNNING;

	return true;

}

// Initialise the ball
ball_struct* init_ball(int size) {

	ball_struct ball = { .x = SCREEN_WIDTH / 2 - size / 2, .y = SCREEN_HEIGHT
			/ 2 - size / 2, .size = size, .xVel = BALLVEL, .yVel = BALLVEL,
			.pX = 0, .pY = 0 };

	ball_struct* ball_ptr = malloc(sizeof(ball_struct));
	ball_ptr->size = ball.size;
	ball_ptr->x = ball.x;
	ball_ptr->xVel = -ball.xVel;
	ball_ptr->y = ball.y;
	ball_ptr->yVel = ball.yVel;
	ball_ptr->pX = ball.x;
	ball_ptr->pY = ball.y;

	return ball_ptr;

}

// Initialise the player
player_pad* init_player_pad(void) {

	player_pad pad = { .yPos = SCREEN_HEIGHT / 2, .score = 0 };

	player_pad* pad_ptr = malloc(sizeof(player_pad));
	pad_ptr->score = pad.score;
	pad_ptr->yPos = pad.yPos;
	pad_ptr->w = PLAYERPAD_WIDTH;
	pad_ptr->h = PLAYERPAD_HEIGHT;

	return pad_ptr;
}

// Update the score and print it to the Serial terminal.
void score_update(player_pad* pad1, int score) {
	pad1->score = pad1->score + score;
}

// Update the CPU player's position
void update_CPU_pad(player_pad* pad2, ball_struct* ball, int t_elapse) {

	pad2->pY = pad2->yPos;
	pad2->pX = pad2->xPos;

	if (ball->x > SCREEN_WIDTH / 2) {
		if (pad2->yPos <= ball->y)
			pad2->yPos += PLAYERPAD_VELOCITY * t_elapse;

		if (pad2->yPos >= ball->y)
			pad2->yPos -= PLAYERPAD_VELOCITY * t_elapse;

	}

	if (pad2->yPos < pad2->h / 2)
		pad2->yPos = pad2->h / 2;

	if (pad2->yPos > SCREEN_HEIGHT - pad2->h / 2)
		pad2->yPos = SCREEN_HEIGHT - pad2->h / 2;

}

// Update the player pad position
void update_player_pad(player_pad* pad1, player_pad* pad2, ball_struct* ball,
		int t_elapse) {

	pad1->pX = pad1->xPos;
	pad1->pY = pad1->yPos;

	if (ball->x < SCREEN_WIDTH / 2) {
		if (pad1->yPos <= ball->y)
			pad1->yPos += PLAYERPAD_VELOCITY * t_elapse;

		if (pad1->yPos >= ball->y)
			pad1->yPos -= PLAYERPAD_VELOCITY * t_elapse;

	}

	if (pad1->yPos < pad1->h / 2)
		pad1->yPos = pad1->h / 2;

	if (pad1->yPos > SCREEN_HEIGHT - pad1->h / 2)
		pad1->yPos = SCREEN_HEIGHT - pad1->h / 2;

}

void check_interesections(game_context* g, graphics_context* gc) {

	// Ball rectangle
	SDL_Rect ballRect = { .x = g->ball->x - g->ball->size / 2, .y = g->ball->y
			+ g->ball->size / 2, .w = g->ball->size, .h = g->ball->size, };

	// Player rectangle
	SDL_Rect pad1Rect = { .x = g->pad1->xPos, .y = g->pad1->yPos, .w =
			g->pad1->w, .h = g->pad1->h };

	// CPU rectangle
	SDL_Rect pad2Rect = { .x = g->pad2->xPos, .y = g->pad2->yPos, .w =
			g->pad1->w, .h = g->pad1->h };

	// Check for intersections
	if (SDL_HasIntersection(&ballRect, &pad1Rect))
		g->ball->xVel = -(g->ball->xVel);

	if (SDL_HasIntersection(&ballRect, &pad2Rect)) {
		g->ball->xVel = -(g->ball->xVel);
	}

//	drawRect(pad1Rect.x, pad1Rect.y, pad1Rect.w, pad1Rect.h, 128, 128, 128, gc);

}

// Update the ball structure
void update_ball(game_context *game, GAME_STATE state, int t_elapse) {

	ball_struct* ball = game->ball;

	if (state == STOPPED) {
		ball->x = SCREEN_WIDTH / 2;
		ball->y = SCREEN_HEIGHT / 2;
		return;
	}

	// Store previous locations
	ball->pX = ball->x;
	ball->pY = ball->y;

	// Update current locations
	ball->x += game->ball->xVel * t_elapse;
	ball->y += game->ball->yVel * t_elapse;

	// Check x-crossings
	if (ball->x < BALLSIZE) {
		ball->xVel = -(ball->xVel);
		score_update(game->pad1, 1);
	}
	if (ball->x > SCREEN_WIDTH - BALLSIZE) {
		score_update(game->pad1, 1);
		ball->xVel = -(ball->xVel);
	}

	// Check y-crossings
	if (ball->y < BALLSIZE) {
		ball->yVel = -(ball->yVel);
	}
	if (ball->y > SCREEN_HEIGHT - BALLSIZE) {
		ball->yVel = -(ball->yVel);
	}

}

// Shutdown the game
void shutdown(game_context* game) {

	free(game->pad1);
	free(game->pad2);
	free(game->ball);
	game->ball = NULL;
	game->pad1 = NULL;
	game->pad2 = NULL;
	game->state = STOPPED;

	return;
}

// This function is called repeatedly to update the game
void update(int t_elapse, game_context* game, graphics_context* gc) {

	update_ball(game, game->state, t_elapse);
	render_ball(game, gc);

	update_player_pad(game->pad1, game->pad2, game->ball, t_elapse);
	update_CPU_pad(game->pad2, game->ball, t_elapse);
	check_interesections(game, gc);
	render_pads(game, gc);

}

void render_pads(game_context* game, graphics_context* gc) {

	drawRect(game->pad1->pX, game->pad1->pY, game->pad1->w, game->pad1->h, 0, 0,
			0, gc);
	drawRect(game->pad1->xPos, game->pad1->yPos, game->pad1->w, game->pad1->h,
			0, 255, 0, gc);

	drawRect(game->pad2->pX, game->pad2->pY, game->pad2->w, game->pad2->h, 0, 0,
			0, gc);
	drawRect(game->pad2->xPos, game->pad2->yPos, game->pad2->w, game->pad2->h,
			0, 0, 255, gc);

}

void render_ball(game_context* game, graphics_context* gc) {

	drawRect(game->ball->pX, game->ball->pY, game->ball->size, game->ball->size,
			0, 0, 0, gc);
	drawRect(game->ball->x, game->ball->y, game->ball->size, game->ball->size,
			255, 0, 0, gc);

}

void print_state(game_context* g) {

	// Print the game state
	xil_printf("STATE: %d\n", g->state);

	// Print the ball's data
	xil_printf("BALL (X,Y) = (%d,%d)\n", g->ball->x, g->ball->y);
	xil_printf("BALL (vX,vY) = (%d,%d)\n", g->ball->xVel, g->ball->yVel);

	// Print the player's data
	xil_printf("PLAYER (X,Y) = (%d,%d)\n", g->pad1->xPos, g->pad1->yPos);
	// Print the CPU's data
	xil_printf("CPU (vX,vY) = (%d,%d)\n", g->pad2->xPos, g->pad2->yPos);

	xil_printf("--------------\n");

}

int setup_stopwatch(XTmrCtr* TimerCounter) {

	int Status = XTmrCtr_Initialize(TimerCounter, TMRCTR_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 1;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	Status = XTmrCtr_SelfTest(TimerCounter, TMRCTR_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 1;
	}

	/*
	 * Setup the timer counters.
	 */
	XTmrCtr_SetOptions(TimerCounter, TMRCTR_DEVICE_ID, 0);

	// Reset to count up from 0 every time the timer starts
	XTmrCtr_SetResetValue(TimerCounter, TMRCTR_DEVICE_ID, 0);

	return 0;

}

int start_stopwatch(XTmrCtr* TmrCtrInstancePtr) {

	XTmrCtr_Start(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	return XTmrCtr_GetValue(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);

}

int end_stopwatch(XTmrCtr* TmrCtrInstancePtr) {

	int answer = XTmrCtr_GetValue(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	XTmrCtr_Stop(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
	return answer;

}

