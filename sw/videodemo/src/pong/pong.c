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

	const float VEL = BALLVEL;
	ball_struct ball = { .x = SCREEN_WIDTH / 2 - size / 2, .y = SCREEN_HEIGHT
			/ 2 - size / 2, .size = size, .xVel = VEL
			* ((rand() % 2 == 1 ? true : false) ? 1 : -1), .yVel = VEL
			* ((rand() % 2 == 1 ? true : false) ? 1 : -1) };

	ball_struct* ball_ptr = malloc(sizeof(ball_struct));
	ball_ptr->size = ball.size;
	ball_ptr->x = ball.x;
	ball_ptr->xVel = ball.xVel;
	ball_ptr->y = ball.y;
	ball_ptr->yVel = ball.yVel;

	return ball_ptr;

}

// Initialise the player
player_pad* init_player_pad(void) {

	player_pad pad = { .yPos = SCREEN_HEIGHT / 2, .score = 0 };

	player_pad* pad_ptr = malloc(sizeof(player_pad));
	pad_ptr->score = pad.score;
	pad_ptr->yPos = pad.yPos;

	return pad_ptr;
}

// Update the score and print it to the Serial terminal.
void score_update(player_pad* pad1, int score) {
	pad1->score = pad1->score + score;
}

// Update the CPU player's position
void update_CPU_pad(player_pad* pad2, ball_struct* ball, float t_elapse) {

	if (ball->x > SCREEN_WIDTH / 2) {
		if (pad2->yPos <= ball->y)
			pad2->yPos += PLAYERPAD_VELOCITY * t_elapse;

		if (pad2->yPos >= ball->y)
			pad2->yPos -= PLAYERPAD_VELOCITY * t_elapse;

	} else {
		pad2->yPos = pad2->yPos;
	}
	if (pad2->yPos < PLAYERPAD_HEIGHT / 2)
		pad2->yPos = PLAYERPAD_HEIGHT / 2;
	if (pad2->yPos > SCREEN_HEIGHT - PLAYERPAD_HEIGHT / 2)
		pad2->yPos = SCREEN_HEIGHT - PLAYERPAD_HEIGHT / 2;

}

// Update the player pad position
void update_player_pad(player_pad* pad1, player_pad* pad2, ball_struct* ball,
		float t_elapse) {

	// This function needs to be modified, heavily. Currently, it is a stub.

	if (pad1->yPos < PLAYERPAD_HEIGHT / 2)
		pad1->yPos = PLAYERPAD_HEIGHT / 2;

	if (pad1->yPos > SCREEN_HEIGHT - PLAYERPAD_HEIGHT / 2)
		pad1->yPos = SCREEN_HEIGHT - PLAYERPAD_HEIGHT / 2;

	//overlaps ball?
	SDL_Rect ballRect = { .x = ball->x - ball->size / 2, .y = ball->y
			- ball->size / 2, .w = ball->size, .h = ball->size, };
	SDL_Rect pad1Rect = { .x = PLAYERPAD_MARGIN, .y = (int) (pad1->yPos)
			- PLAYERPAD_HEIGHT / 2, .w = PLAYERPAD_WIDTH, .h =
	PLAYERPAD_HEIGHT, };

	if (SDL_HasIntersection(&ballRect, &pad1Rect))
		ball->xVel = fabs(ball->xVel);

	SDL_Rect pad2Rect = {
			.x = SCREEN_WIDTH - PLAYERPAD_WIDTH - PLAYERPAD_MARGIN, .y =
					(int) (pad2->yPos) - PLAYERPAD_HEIGHT / 2, .w =
			PLAYERPAD_WIDTH, .h = PLAYERPAD_HEIGHT, };

	if (SDL_HasIntersection(&ballRect, &pad2Rect))
		ball->xVel = -fabs(ball->xVel);

}

// Update the ball structure
void update_ball(game_context *game, GAME_STATE state, float t_elapse) {

	ball_struct* ball = game->ball;

	if (state == STOPPED) {
		ball->x = SCREEN_WIDTH / 2;
		ball->y = SCREEN_HEIGHT / 2;
		return;
	}
	ball->x += game->ball->xVel * t_elapse;
	ball->y += game->ball->yVel * t_elapse;

	if (ball->x < BALLSIZE / 2)
		score_update(game->pad1, 1); //ball->xVel = fabs(ball->xVel);
	if (ball->y < BALLSIZE / 2)
		ball->yVel = fabs(ball->yVel);
	if (ball->x > SCREEN_WIDTH - BALLSIZE / 2)
		score_update(game->pad1, 1); //ball->xVel = -fabs(ball->xVel);
	if (ball->y > SCREEN_HEIGHT - BALLSIZE / 2)
		ball->yVel = -fabs(ball->yVel);

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
void update(float t_elapse, game_context* game, graphics_context* gc) {

	update_ball(game, game->state, t_elapse);
	render_ball(game, gc);

	update_player_pad(game->pad1, game->pad2, game->ball, t_elapse);
	update_CPU_pad(game->pad2, game->ball, t_elapse);
	render_pads(game, gc);

}

void render_pads(game_context* game, graphics_context* gc) {

	drawRect(game->pad1->xPos, game->pad1->yPos, 10, 100, 0,255,0, gc);
	drawRect(game->pad2->xPos, game->pad2->yPos, 10, 100, 0,0,255, gc);

}

void render_ball(game_context* game, graphics_context* gc) {

	drawRect(game->ball->x, game->ball->y, game->ball->size, game->ball->size, 255,0,0, gc);

}

